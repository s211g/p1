#pragma once

#include <iostream>

namespace template_specialization_test {

    template <typename>
    class X {
    public:
        X();
        void f();
    };

    template <typename T>
    X<T>::X() { std::cout << "X<T>::X()" << std::endl; }

    template <typename T>
    void X<T>::f() { std::cout << "X<T>::f()" << std::endl; }

    template <>
    class X<int> {
    public:
        X();
        void f();
        template <typename T>
        void ft();
    };

    // если в определении нет шаблоа, то код должен быть в cpp файле
    // в заголовок нельзя выносить реализацию, будет ошибка  multiple definition
    //X<int>::X() {}
    //void X<int>::f() {}

    // если в определении есть шаблон с неопределенным typename(class), то код должен быть в заголовке
    // template <> - не надо
    template <typename T>
    void X<int>::ft() { std::cout << "X<int>::ft()" << std::endl; }

    template <typename T>
    void f(T t) { std::cout << "ft<T>(T t)" << std::endl; }

    void test_outside_definitions();
    void test_explicit_specialization();

    inline void test() {
        std::cout << "test_explicit_specialization" << std::endl;

        test_outside_definitions();
        //test_explicit_specialization();
    }
}