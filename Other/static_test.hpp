#pragma once

#include <iostream>

namespace static_test {


    class A {
    public:
        A(int i_) :
            i(i_) { std::cout << "A::A() i = " << i << std::endl; }
        A() { std::cout << "A::A()" << std::endl; }
        ~A() { std::cout << "~A::A() i = " << i << std::endl; }
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

    void test1();
    void test2();

    inline void test() {
        std::cout << "static tests" << std::endl;

        test1();
        test2();
    }
}
