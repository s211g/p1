#pragma once

#include <atomic>

namespace ThreadSafeStack {

    // base -------------------------------------------------------

    template <typename T>
    struct LockFreeStackNode {
        T data;
        LockFreeStackNode* next;
        LockFreeStackNode(T const& data_) :
            data(data_) {}
    };

    template <typename T>
    struct LockFreeStackSNode {
        std::shared_ptr<T> data;
        LockFreeStackSNode* next;
        LockFreeStackSNode(T const& data_) :
            data(std::make_shared<T>(data_)) {}
    };

    template <typename T, typename Node>
    class LockFreeStackBase {
    protected:
        std::atomic<Node*> head;

    public:
        void push(T const& data) {
            Node* const new_node = new Node(data);
            new_node->next       = head.load();
            while (!this->head.compare_exchange_weak(new_node->next, new_node))
                ;
        }

        Node* pop_head() {
            Node* old_head = head.load();
            while (old_head && !head.compare_exchange_weak(old_head, old_head->next))
                ;
            return old_head;
        }

        bool empty() {
            return !head.load();
        }
    };

    // base -------------------------------------------------------

    // X writers, 1 Reader
    template <typename T, typename Node = LockFreeStackNode<T>, typename Base = LockFreeStackBase<T, Node>>
    class LockFreeStackXW1R : public Base {
        using Base::head;
        using Base::pop_head;

    public:
        bool pop(T& data) {
            Node* old_head = pop_head();
            if (!old_head)
                return false;
            data = old_head->data;
            delete old_head;
            return true;
        }
    };

    // X writers, 1 Reader
    template <typename T, typename Node = LockFreeStackSNode<T>, typename Base = LockFreeStackBase<T, Node>>
    class LockFreeStackSXW1R : public Base {
        using Base::head;
        using Base::pop_head;

    public:
        std::shared_ptr<T> popS() {
            Node* old_head = pop_head();
            if (old_head) {
                std::shared_ptr<T> data = old_head->data;
                delete old_head;
                return data;
            }
            return std::shared_ptr<T>();
        }

        bool empty() {
            return !head.load();
        }
    };

    template <typename T, typename Node = LockFreeStackNode<T>, typename Base = LockFreeStackBase<T, Node>>
    class LockFreeStack : public Base {
        using Base::head;
        using Base::pop_head;

    public:
        bool pop(T& data) {
            Node* old_head = pop_head();
            if (!old_head)
                return false;
            data = old_head->data;
            delete old_head;
            return true;
        }
    };

    template <typename T, typename Node = LockFreeStackSNode<T>, typename Base = LockFreeStackBase<T, Node>>
    class LockFreeStackS : public Base {
        using Base::head;
        using Base::pop_head;

    public:
        std::shared_ptr<T> popS() {
            Node* old_head = pop_head();
            if (old_head) {
                std::shared_ptr<T> data = old_head->data;
                delete old_head;
                return data;
            }
            return std::shared_ptr<T>();
        }

        bool empty() {
            return !head.load();
        }
    };

}