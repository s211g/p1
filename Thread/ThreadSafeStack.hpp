#pragma once

#include <atomic>

namespace ThreadSafeStack {

    template <typename T>
    class LockFreeStack {
    private:
        struct node {
            T data;
            node* next;
            node(T const& data_) :
                data(data_) {}
        };

        std::atomic<node*> head;

    public:
        void push(T const& data) {
            node* const new_node = new node(data);
            new_node->next       = head.load();
            while (!head.compare_exchange_weak(new_node->next, new_node))
                ;
        }

        bool pop(T& data) {
            node* old_head = head.load();
            while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
                ;
            if (!old_head)
                return false;
            data = old_head->data;
            return true;
        }

        bool empty() {
            return !head.load();
        }
    };

    template <typename T>
    class LockFreeStackS {
    private:
        struct node {
            std::shared_ptr<T> data;
            node* next;
            node(T const& data_) :
                data(std::make_shared<T>(data_)) {}
        };

        std::atomic<node*> head;

    public:
        void push(T const& data) {
            node* const new_node = new node(data);
            new_node->next       = head.load();
            while (!head.compare_exchange_weak(new_node->next, new_node))
                ;
        }

        std::shared_ptr<T> popS() {
            node* old_head = head.load();
            while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
                ;
            return old_head ? old_head->data : std::shared_ptr<T>();
        }

        bool empty() {
            return !head.load();
        }
    };
}