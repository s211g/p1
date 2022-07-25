#pragma once

#include <iostream>
#include <ostream>

namespace vatemplate_test {

    void test_sum17();

    template <typename... T>
    auto sum17(const T&... x) {
        return (x + ...);
    }

    void test_sum14();

    template <typename T1>
    auto sum(const T1& x1) {
        std::cout << "sum(const T1& x1) x1 = " << x1 << std::endl;
        return x1;
    }

    template <typename T1, typename... T>
    auto sum(const T1& x1, const T&... x) {
        std::cout << "sum(const T1& x1, const T&... x) x1 = " << x1 << std::endl;
        return x1 + sum(x...);
    }

    void test_sum14_constexpr();

    template <size_t...>
    constexpr size_t sum14_constexpr = 0;

    template <size_t first, size_t... other>
    constexpr size_t sum14_constexpr<first, other...> = first + sum14_constexpr<other...>;

    template <typename... types>
    constexpr size_t sum14_constexpr_sizeof = sum14_constexpr<sizeof(types)...>;

    void test_print();

    inline void print() { std::cout << "print() " << std::endl; }                          // Нечего печатать.
    inline void print(std::ostream&) { std::cout << "print(std::ostream&)" << std::endl; } // Нечего печатать.

    template <class First, class... Other>
    void print(std::ostream& os,
               const First& first, const Other&... other) {
        os << first;
        print(os, other...);
    }

    template <class First, class... Other>
    void print(const First& first, const Other&... other) {
        std::cout << first;
        print(other...);
    }

    void test_tgroup();

    template <typename... T>
    struct Group;

    template <typename T1>
    struct Group<T1> {
        T1 t1_;
        Group() = default;
        explicit Group(const T1& t1) :
            t1_(t1) {
            std::cout << "Group<T1>::Group(const T1& t1) t1 = " << t1_ << std::endl;
        }
        explicit Group(T1&& t1) :
            t1_(std::move(t1)) {
            std::cout << "Group<T1>::Group(const T1&& t1) t1 = " << t1_ << std::endl;
        }
        explicit operator const T1&() const {
            std::cout << "Group<T1>::operator const T1&() t1 = " << t1_ << std::endl;
            return t1_;
        }
        explicit operator T1&() {
            std::cout << "Group<T1>::operator T1&() t1 = " << t1_ << std::endl;
            return t1_;
        }
    };

    template <typename T1, typename... T>
    struct Group<T1, T...> : Group<T...> {
        T1 t1_;
        Group() = default;
        explicit Group(const T1& t1, T&&... t) :
            Group<T...>(std::forward<T>(t)...), t1_(t1) {
            std::cout << "Group<T1, T...>::Group(const T1& t1,  T&&... t) t1 = " << t1_ << std::endl;
        }
        explicit Group(T1&& t1, T&&... t) :
            Group<T...>(std::forward<T>(t)...), t1_(std::move(t1)) {
            std::cout << "Group<T1, T...>::Group(const T1&& t1,  T&&... t) t1 = " << t1_ << std::endl;
        }
        explicit operator const T1&() const {
            std::cout << "Group<T1, T...>::operator const T1&() t1 = " << t1_ << std::endl;
            return t1_;
        }
        explicit operator T1&() {
            std::cout << "Group<T1, T...>::operator T1&() t1 = " << t1_ << std::endl;
            return t1_;
        }
    };

    template <typename... T>
    auto makeGroup(T&&... t) {
        return Group<T...>(std::forward<T>(t)...);
    }

    void test_overload_set();

    template <typename... F>
    struct overload_set;

    template <typename F1>
    struct overload_set<F1> : public F1 {
        overload_set(F1&& f1) :
            F1(std::move(f1)) {}
        overload_set(const F1& f1) :
            F1(f1) {}
        using F1::operator();
    };

    template <typename F1, typename... F>
    struct overload_set<F1, F...> : public F1, public overload_set<F...> {
        overload_set(F1&& f1, F&&... f) :
            F1(std::move(f1)), overload_set<F...>(std::forward<F>(f)...) {}
        overload_set(const F1& f1, F&&... f) :
            F1(f1), overload_set<F...>(std::forward<F>(f)...) {}
        using F1::operator();
    };

    template <typename... F>
    auto overload(F&&... f) {
        return overload_set<F...>(std::forward<F>(f)...);
    }

    void test_overload_set17();

    template <typename... F>
    struct overload_set17 : public F... {
        overload_set17(F&&... f) :
            F(std::forward<F>(f))... {}
        using F::operator()...;
    };

    template <typename... F>
    auto overload17(F&&... f) {
        return overload_set17<F...>(std::forward<F>(f)...);
    }

    void test_forward();

    class A {
    public:
        int i;

        /*
        A(int i_) :
            i(i_) {
            std::cout << "A(int i)" << std::endl;
        }
*/
        A(int& i_) :
            i(i_) {
            std::cout << "A(int& i)" << std::endl;
        }

        A(const int& i_) :
            i(i_) {
            std::cout << "A(const int& i)" << std::endl;
        }

        A(int&& i_) :
            i(i_) {
            std::cout << "A(int&& i)" << std::endl;
        }

        A() {
            std::cout << "A()" << std::endl;
        }

        ~A() = default;

        A(const A& t) {
            std::cout << "A(const T& t)" << std::endl;
        }

        A(A& t) {
            std::cout << "A(T& t)" << std::endl;
        }

        A(A&& t) {
            std::cout << "A(T&& t)" << std::endl;
        }

        A& operator=(A& a) {
            std::cout << "A::operator(T& t)" << std::endl;
            return *this;
        }

        A& operator=(const A& a) {
            std::cout << "A::operator(T& t)" << std::endl;
            return *this;
        }

        void print() const {
            std::cout << "this[" << this << "] i = " << i << std::endl;
        }
    };

    template <typename T>
    void f(T&& t) {
        T t1 = std::forward<T>(t);
        t1.print();
    }

    template <typename T>
    void f2(T&& t) {
        T t1(std::forward<T>(t));
        t1.print();
    }

    template <typename T>
    void f4(T&& t) {
        A a(std::forward<T>(t));
    }

    template <typename T>
    bool less(T&& t1, T&& t2) {
        return t1 < t2;
    }

    inline bool less2(int&& t1, int&& t2) {
        return t1 < t2;
    }

    template <typename T>
    bool less3(T&& t1, T&& t2) {
        return t1 < t2;
    }

    inline void
    test() {
        std::cout << "variable arguments of template tests" << std::endl;

        //test_sum17();
        //test_sum14();
        //test_sum14_constexpr();
        test_print();
        //test_tgroup();
        //test_overload_set();
        //test_overload_set();
        //test_overload_set17();
        //test_forward();
    }
}