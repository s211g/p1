#pragma once

#include <iostream>

namespace template_explicit_specialization_test {

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

        //struct B {}; // класс-член
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

    // test_partial_specialization ----------------------------------------------------------

    // обобщенный шаблон, только объявление без реализации
    template <typename T1, typename T2>
    class P;

    template <typename T1>
    class P<T1, int> {
    public:
        P() { std::cout << "P<T1, int>::P()" << std::endl; }
    };

    template <typename T2>
    class P<int, T2> {
    public:
        P() { std::cout << "P<int, T2>::P()" << std::endl; }
    };

    template <>
    class P<int, int> {
    public:
        explicit P(int) { std::cout << "P<int, int>::P(int)" << std::endl; }
        P() { std::cout << "P<int, int>::P()" << std::endl; }
    };

    template <typename T1>
    class P<T1*, int> {
    public:
        P() { std::cout << "P<T*, int>::P()" << std::endl; }
        void f(T1 t) { std::cout << "t : " << t << std::endl; }
    };

    template <typename T1>
    class P<T1&, int> {
    public:
        P() { std::cout << "P<T1&, int>::P()" << std::endl; }
        //explicit P(T1&) { std::cout << "P<T1&, int>::P(T1&)" << std::endl; }
        void f(T1 t) { std::cout << "t : " << t << std::endl; }
    };

    template <typename T1, typename T2>
    class P<T1*, T2*> {
    public:
        P() { std::cout << "P<T*, T*>::P()" << std::endl; }
    };

    template <typename T1, typename T2>
    class P<T1&, T2&> {
    public:
        P() { std::cout << "P<T&, T&>::P()" << std::endl; }
    };

    template <typename T>
    class E {
    public:
        E() { std::cout << "E<T>::E()" << std::endl; }
        explicit E(T&) { std::cout << "E<T>::E(T&)" << std::endl; }
        explicit E(T*) { std::cout << "E<T>::E(T*)" << std::endl; }
        //E(T&) { std::cout << "E<T>::E(T&)" << std::endl; }
    };

    template <>
    class E<int> {
    public:
        E() { std::cout << "E<int>::E()" << std::endl; }
        explicit E(int&) { std::cout << "E<int>::E(int&)" << std::endl; }
        explicit E(int*) { std::cout << "E<int>::E(int*)" << std::endl; }
    };

    template <>
    class E<int*> {
    public:
        E() { std::cout << "E<int*>::E()" << std::endl; }
        explicit E(int&) { std::cout << "E<int*>::E(int&)" << std::endl; }
        explicit E(int*) { std::cout << "E<int*>::E(int*)" << std::endl; }
    };

    // test_partial_specialization ----------------------------------------------------------

    template <typename T1, typename T2>
    class F {
    public:
        F(T1 t1, T2 t2) { std::cout << "F<T1,T2>(" << t1 << ", " << t2 << ")" << std::endl; }
    };

    void test_outside_definitions();
    void test_explicit_specialization();
    void test_partial_specialization();

    inline void test() {
        std::cout << "template_explicit_specialization_test" << std::endl;

        //test_explicit_outside_definitions();
        //test_explicit_specialization();
        test_partial_specialization();
    }
}