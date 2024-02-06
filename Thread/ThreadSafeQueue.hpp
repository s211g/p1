#pragma once

#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>


namespace ThreadSafeQueue {

    template <typename T>
    class ThreadSafeQueue {
    private:
        mutable std::mutex m_mtx;
        std::queue<T> m_queue;
        std::condition_variable m_cv;

    public:
        ThreadSafeQueue() {}
        void push(T value) {
            std::lock_guard lock(m_mtx);
            m_queue.push(std::move(value));
            m_cv.notify_one();
        }

        bool pop(T& value) {
            std::lock_guard lock(m_mtx);
            if (m_queue.empty())
                return false;
            value = std::move(m_queue.front());
            m_queue.pop();
            return true;
        }

        bool pop(T& value, uint32_t timeout_ms) {
            std::unique_lock lock(m_mtx);
            if (m_cv.wait_for(lock, std::chrono::milliseconds(timeout_ms), [this] { return !m_queue.empty(); })) {
                value = std::move(m_queue.front());
                m_queue.pop();
                return true;
            }
            return false;
        }

        bool empty() {
            std::lock_guard lock(m_mtx);
            return m_queue.empty();
        }
    };

    // head = node -> node -> node = tail
    template <typename T>
    class ThreadSafeQueueS {
    private:
        struct node {
            std::shared_ptr<T> data;
            std::unique_ptr<node> next;
        };
        mutable std::mutex head_mutex;
        std::unique_ptr<node> head;
        mutable std::mutex tail_mutex;
        node* tail;
        std::condition_variable data_cond;

    public:
        ThreadSafeQueueS() :
            head(new node), tail(head.get()) {}
        ThreadSafeQueueS(const ThreadSafeQueueS&) = delete;
        ThreadSafeQueueS& operator=(const ThreadSafeQueueS&) = delete;

    private:
        node* get_tail() {
            std::lock_guard<std::mutex> lock(tail_mutex);
            return tail;
        }
        std::unique_ptr<node> pop_head() {
            std::unique_ptr<node> old_head = std::move(head);
            head                           = std::move(old_head->next);
            return old_head;
        }

    public:
        void pushS(std::shared_ptr<T>& new_data) {
            std::unique_ptr<node> new_node(new node);
            {
                std::lock_guard<std::mutex> lock(tail_mutex);
                tail->data           = new_data;
                node* const new_tail = new_node.get();
                tail->next           = std::move(new_node);
                tail                 = new_tail;
            }
            data_cond.notify_one(); // проверить сколько тредов проснуца если вызвать несколько раз подряд потом ткнуца на вычитывание
        }

        void push(T& data) {
            std::shared_ptr<T> new_data = std::make_shared<T>(data);
            pushS(new_data);
        }

        std::shared_ptr<T> popS(uint32_t timeout_ms) {
            std::unique_lock<std::mutex> lock(head_mutex);
            if (!timeout_ms) {
                if (head.get() == get_tail())
                    return std::unique_ptr<T>();
                return pop_head()->data;
            }

            if (data_cond.wait_for(lock, std::chrono::milliseconds(timeout_ms), [&] { return head.get() != get_tail(); })) {
                return pop_head()->data;
            }
            return std::unique_ptr<T>();
        }

        bool pop(T& data, uint32_t timeout_ms = 0) {
            std::unique_lock<std::mutex> lock(head_mutex);

            if (!timeout_ms) {
                if (head.get() == get_tail())
                    return false;

                data = std::move(*pop_head()->data);
                return true;
            }

            if (data_cond.wait_for(lock, std::chrono::milliseconds(timeout_ms), [&] { return head.get() != get_tail(); })) {
                data = std::move(*pop_head()->data);
                return true;
            }
            return false;
        }

        bool empty() {
            std::unique_lock<std::mutex> lock(head_mutex);
            return head.get() == get_tail();
        }
    };


    // 1 writers, 1 Reader
    template <typename T>
    class LockFreeQueueS1W1R {
    private:
        struct node {
            std::shared_ptr<T> data;
            node* next;
            node() :
                next(nullptr) {}
        };

        std::atomic<node*> head;
        std::atomic<node*> tail;

        node* pop_head() {
            node* const old_head = head.load();
            if (old_head == tail.load())
                return nullptr;
            head.store(old_head->next);
            return old_head;
        }

    public:
        LockFreeQueueS1W1R() :
            head(new node), tail(head.load()) {}
        LockFreeQueueS1W1R(const LockFreeQueueS1W1R&) = delete;
        LockFreeQueueS1W1R& operator=(const LockFreeQueueS1W1R&) = delete;
        ~LockFreeQueueS1W1R() {
            while (node* const old_node = head.load()) {
                head.store(old_node->next);
                delete old_node;
            }
        }

        void push(T& data) {
            std::shared_ptr<T> new_data = std::make_shared<T>(data);
            node* new_node              = new node;
            node* const old_tail        = tail.load();
            // tail фиктивный, заполняем его реальными данными , ->next = на новый пустой фиктивный tail, новый фиктивный tail в объект tail
            old_tail->data.swap(new_data);
            old_tail->next = new_node;
            tail.store(new_node);
        }

        bool pop(T& data) {
            node* old_head = pop_head();
            if (old_head) {
                data = std::move(*old_head->data);
                delete old_head;
                return true;
            }
            return false;
        }

        std::shared_ptr<T> popS() {
            node* old_head = pop_head();
            std::shared_ptr<T> data;
            if (old_head) {
                data.swap(old_head->data);
                delete old_head;
            }
            return data;
        }

        bool empty() {
            return head.load() == tail.load();
        }
    };
}