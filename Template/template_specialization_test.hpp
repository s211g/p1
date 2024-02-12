#pragma once

#include <iostream>

namespace template_specialization_test {

    // ---------------- шаблон класса -------------------

    template <typename T>
    class X {
    public:
        X();

        // функция-член
        void f();
        void f1();

        // шаблон функции-член
        template <typename T2>
        void ft();

        // объявление типа структуры B внутри пространства X<T>
        struct B;
        //struct B { void f(); };
        // класс-член
        B b;

        struct D {
            void f() { std::cout << "X<T>::D::f()" << std::endl; }
        };

        // шаблон класса-члена
        template <class U>
        struct C {};

        // статический член
        static int s;
    };

    template <typename T>
    X<T>::X() { std::cout << "X<T>::X()" << std::endl; }

    // функция-член ---------------------------------------------------------
    template <typename T>
    void X<T>::f() { std::cout << "X<T>::f()" << std::endl; }
    template <typename T>
    void X<T>::f1() { std::cout << "X<T>::f1()" << std::endl; }

    // шаблон функции-член --------------------------------------------------
    template <typename T>
    template <typename T2>
    void X<T>::ft() { std::cout << "X<T>::ft()" << std::endl; }

    // класс-член -----------------------------------------------------------
    // определение типа структуры B внутри пространства X<T>
    template <typename T>
    struct X<T>::B {
        void f();
    };

    // определение функции X<T>::B::f()
    template <typename T>
    void X<T>::B::f() { std::cout << "X<T>::B::f()" << std::endl; }

    // явная специализация содержимого структуры X<char>::D общего шаблона
    template <>
    struct X<float>::D {
        void g() { std::cout << "X<float>::D::g()" << std::endl; }
    };

    // ---------------- шаблон класса(явная специализация) -------------------

    template <>
    class X<int> {
    public:
        X();
        void f();
        template <typename T2>
        void ft();

        struct B {}; // класс-член
    };

    // при явной(полной) специализации реализация функций в не класса должна находиться в .cpp файле
    //X<int>::X() {}


    // функция-член ---------------------------------------------------------
    //void X<int>::f() {}

    // шаблон функции-член --------------------------------------------------
    // если в определении есть шаблон с неопределенным typename(class), то код должен быть в заголовке
    // template <> - не надо
    template <typename T2>
    void X<int>::ft() { std::cout << "X<int>::ft()" << std::endl; }


    // класс-член -----------------------------------------------------------
    // //template <> // специализация класса-члена
    // class X<int>::B {
    //     void f();
    // };

    // // template<> также не используется для члена специализированного класса-члена
    // void X<int>::B::f() {
    // }

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
    // объявление типа свободной функции, не является специализацией
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