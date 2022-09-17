#pragma once

#include <queue>
#include <mutex>
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

    // head -> node -> node -> node <-tail
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
}