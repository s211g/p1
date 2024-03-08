#pragma once

#include <iostream>
#include <atomic>

//#define STATIC_TEST_ENABLED

namespace static_test {

    class A {
    public:
        A(int i_) :
            i(i_) {
#ifndef STATIC_TEST_ENABLED
            return;
#endif
            std::cout << "A::A() i = " << i << std::endl;
        }
        A() {
#ifndef STATIC_TEST_ENABLED
            return;
#endif
            std::cout << "static_test A::A()" << std::endl;
        }
        ~A() {
#ifndef STATIC_TEST_ENABLED
            return;
#endif
            std::cout << "static_test ~A::A() i = " << i << std::endl;
        }
        int i{777};
    };

    class B {
    public:
        B() { std::cout << "B::B()" << std::endl; }
        ~B() { std::cout << "~B::B()" << std::endl; }
        static A a;

        static void printJK(const B& b) {
            std::cout << "b.j = " << b.j << " b.k = " << b.k << std::endl;
        }

    private:
        int j{987};

    private:
        int k{987};
    };

    template <typename T>
    class C {
    public:
        static int i;
    };

    // статический член(для int определение в cpp)
    template <typename T>
    int C<T>::i{-10};

    void test1();
    void test2();
    void test_static_in_template();

    inline void test() {
        //#define STATIC_TEST_ENABLED
        std::cout << "static tests" << std::endl;

        //test1();
        //test2();
        test_static_in_template();
    }
}
