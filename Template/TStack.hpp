#pragma once

#include <deque>
#include <cassert>
#include <memory>

namespace template_template_specialization_test {

    template <typename T,
              template <typename Elem, typename = std::allocator<Elem>> class Cont = std::deque>
    class TStack {
    private:
        Cont<T> elems;

    public:
        void push(T const&);
        void pop();
        T top();
        bool empty();

        // присваивание стека с другим типом
        template <typename T2,
                  template <typename Elem2, typename = std::allocator<Elem2>> class Cont2>
        TStack<T, Cont>& operator=(TStack<T2, Cont2> const&);

        // для доступа к закрытым членам для любого типа стека
        template <typename, template <typename, typename> typename>
        friend class TStack;
    };

    template <typename T, template <typename, typename> typename Cont>
    void TStack<T, Cont>::push(T const& elem) {
        elems.push_back(elem);
    }

    template <typename T, template <typename, typename> typename Cont>
    void TStack<T, Cont>::pop() {
        if (!elems.empty())
            elems.pop_back();
    }

    template <typename T, template <typename, typename> typename Cont>
    T TStack<T, Cont>::top() {
        assert(!elems.empty());
        return elems.back();
    }

    template <typename T, template <typename, typename> typename Cont>
    bool TStack<T, Cont>::empty() {
        return elems.empty();
    }

    template <typename T, template <typename, typename> typename Cont>
    template <typename T2,
              template <typename Elem2, typename = std::allocator<Elem2>> class Cont2>
    TStack<T, Cont>& TStack<T, Cont>::operator=(TStack<T2, Cont2> const& op2) {
        elems.clear();
        for (auto& e : op2.elems)
            elems.push_back(static_cast<T>(e));
        //elems.insert(elems.begin(), op2.elems.begin(), op2.elems.end());
        return *this;
    }
}