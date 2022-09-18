#pragma once

#include <mutex>
#include <memory>

namespace ThreadSafeList {

    // head -> node -> node -> node
    template <typename T>
    class ThreadSafeList {
    private:
        struct node {
            std::mutex m;
            std::shared_ptr<T> data;
            std::unique_ptr<node> next;
            node() :
                next() {}
            node(T const& value) :
                data(std::make_shared<T>(value)) {}
        };

        node head;

    public:
        ThreadSafeList() {}
        ~ThreadSafeList() {
            remove_if([](T const&) { return true; });
        }
        ThreadSafeList(const ThreadSafeList&) = delete;
        ThreadSafeList& operator=(const ThreadSafeList&) = delete;

        void push_front(T const& value) {
            std::unique_ptr<node> new_node(new node(value));
            std::lock_guard lock(head.m);
            new_node->next = std::move(head.next);
            head.next      = std::move(new_node);
        }

        template <typename Function>
        void for_each(Function f) {
            node* current = &head;
            std::unique_lock lock(head.m);
            while (node* const next = current->next.get()) {
                std::unique_lock lock_next(next->m);
                lock.unlock();
                f(*next->data);
                current = next;
                lock    = std::move(lock_next);
            }
        }

        size_t size() {
            size_t count{0};
            node* current = &head;
            std::unique_lock lock(head.m);
            while (node* const next = current->next.get()) {
                std::unique_lock lock_next(next->m);
                lock.unlock();
                ++count;
                current = next;
                lock    = std::move(lock_next);
            }
            return count;
        }

        template <typename Predicate>
        std::shared_ptr<T> find_first_if(Predicate p) {
            node* current = &head;
            std::unique_lock lock(head.m);
            while (node* const next = current->next.get()) {
                std::unique_lock lock_next(next->m);
                lock.unlock();
                if (p(*next->data))
                    return next->data;
                current = next;
                lock    = std::move(lock_next);
            }
            return std::shared_ptr<T>();
        }

        template <typename Predicate>
        size_t remove_if(Predicate p) {
            size_t count{0};
            node* current = &head;
            std::unique_lock lock(head.m);
            while (node* const next = current->next.get()) {
                std::unique_lock lock_next(next->m);
                if (p(*next->data)) {
                    std::unique_ptr<node> old_node = std::move(current->next);
                    current->next                  = std::move(next->next);
                    lock_next.unlock();
                    ++count;
                }
                else {
                    lock.unlock();
                    current = next;
                    lock    = std::move(lock_next);
                }
            }
            return count;
        }
    };
}