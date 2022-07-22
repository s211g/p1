#pragma once

#include <iostream>

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

    inline void
    test() {
        std::cout << "variable arguments of template tests" << std::endl;

        //test_sum17();
        //test_sum14();
        //test_tgroup();
        //test_overload_set();
        //test_overload_set();
        test_overload_set17();
    }
}