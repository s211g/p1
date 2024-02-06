#pragma once

#include <atomic>
#include <memory>

namespace ThreadSafeStack {

    // base -------------------------------------------------------

    template <typename T>
    struct LockFreeStackNode {
        T data;
        LockFreeStackNode* next;
        LockFreeStackNode(T const& data_) :
            data(data_) {}
        //T& getData() { return data; }
        //std::shared_ptr<T> getDataS() { return std::make_shared<T>(data); }
    };

    template <typename T>
    struct LockFreeStackSNode {
        std::shared_ptr<T> data;
        LockFreeStackSNode* next;
        LockFreeStackSNode(const T & data_) :
            data(std::make_shared<T>(data_)) {}
        //T& getData() { return *data; }
        //std::shared_ptr<T> getDataS() { return std::move(data); }
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

    template <typename Node>
    class LockFreeStackNodeDeleter {
    protected:
        std::atomic<unsigned> threads_in_pop{0};
        std::atomic<Node*> to_be_deleted;
        void try_reclaim(Node* node) {
            if (threads_in_pop == 1) {
                Node* nodes_to_delete = to_be_deleted.exchange(nullptr);
                if (!--threads_in_pop)
                    delete_nodes(nodes_to_delete);
                else if (nodes_to_delete) {
                    chain_pending_nodes(nodes_to_delete);
                }
                if (node)
                    delete node;
            }
            else {
                if (node)
                    chain_pending_nodes(node, node);
                --threads_in_pop;
            }
        }

        void chain_pending_nodes(Node* nodes) {
            Node* last = nodes;
            while (Node* const next = last->next) {
                last = next;
            }
            chain_pending_nodes(nodes, last);
        }

        void chain_pending_nodes(Node* first, Node* last) {
            last->next = to_be_deleted;
            while (!to_be_deleted.compare_exchange_weak(last->next, first))
                ;
        }

        static void delete_nodes(Node* nodes) {
            while (nodes) {
                Node* node = nodes->next;
                delete nodes;
                nodes = node;
            }
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

    template <typename T, typename Node = LockFreeStackNode<T>, typename Base = LockFreeStackBase<T, Node>, typename Deleter = LockFreeStackNodeDeleter<Node>>
    class LockFreeStack : public Base, public Deleter {
        using Base::head;
        using Base::pop_head;
        using Deleter::threads_in_pop;
        using Deleter::try_reclaim;

    public:
        bool pop(T& data) {
            ++threads_in_pop;
            Node* old_head = pop_head();
            if (old_head)
                data = old_head->data;
            try_reclaim(old_head);
            return old_head;
        }
    };

    template <typename T, typename Node = LockFreeStackSNode<T>, typename Base = LockFreeStackBase<T, Node>, typename Deleter = LockFreeStackNodeDeleter<Node>>
    class LockFreeStackS : public Base, public Deleter {
        using Base::head;
        using Base::pop_head;
        using Deleter::threads_in_pop;
        using Deleter::try_reclaim;

    public:
        std::shared_ptr<T>
        popS() {
            ++threads_in_pop;
            Node* old_head = pop_head();
            std::shared_ptr<T> data;
            if (old_head)
                data.swap(old_head->data);
            try_reclaim(old_head);
            return data;
        }

        bool empty() {
            return !head.load();
        }
    };

}