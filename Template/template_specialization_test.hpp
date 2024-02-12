#pragma once

#include <iostream>

namespace template_specialization_test {

    // ---------------- шаблон класса -------------------

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

    // при явной(полной) специализации реализация функций в не класса должна находиться в .cpp файле
    //X<int>::X() {}
    //void X<int>::f() {}

    // если в определении есть шаблон с неопределенным typename(class), то код должен быть в заголовке
    // template <> - не надо
    template <typename T>
    void X<int>::ft() { std::cout << "X<int>::ft()" << std::endl; }

    // ---------------- шаблон класса -------------------

    // ---------------- шаблон функции -------------------

    // определение общего шаблона
    template <typename T>
    void f(T t) { std::cout << "ft<T>(T t)" << std::endl; }
    // объявление сигнатур частичной специализации
    template <>
    void f<std::string>(std::string t);
    template <>
    void f(char t);
    // объявление типа свободной функции
    void f(double t);

    // ---------------- шаблон функции -------------------


    void test_outside_definitions();
    void test_explicit_specialization();

    inline void test() {
        std::cout << "test_explicit_specialization" << std::endl;

        test_outside_definitions();
        //test_explicit_specialization();
    }
}