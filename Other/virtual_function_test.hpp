#pragma once

#include <iostream>

namespace virtual_function_test {

    class A {
    public:
        int a{1};
        A() {
            std::cout << "A::A()" << std::endl;
            f();
        }

        A(const A&) {
            std::cout << "A::A(const A&)" << std::endl;
        }

        ~A() { std::cout << "A::~A()" << std::endl; }

        virtual void f() { std::cout << "A::f()" << std::endl; }
    };

    class B {
    public:
        int b{2};
        B() {
            std::cout << "B::B()" << std::endl;
            f();
        }
        ~B() { std::cout << "B::~B()" << std::endl; }

        virtual void f() { std::cout << "B::f()" << std::endl; }
    };

    class C : public B, public A {
    public:
        int c{3};
        C() {
            std::cout << "C::C()" << std::endl;
            f();
        }
        ~C() { std::cout << "C::~C()" << std::endl; }

        virtual void f() { std::cout << "C::f()" << std::endl; }
    };

    class BA : public A {
    public:
        int b{2};
        BA() {
            std::cout << "BA::BA()" << std::endl;
            f();
        }
        ~BA() { std::cout << "BA::~BA()" << std::endl; }

        virtual void f() { std::cout << "BA::f()" << std::endl; }
    };

    class C_BA : public BA {
    public:
        int c{3};
        C_BA() {
            std::cout << "C_BA::C_BA()" << std::endl;
            f();
        }
        ~C_BA() { std::cout << "C_BA::~C_BA()" << std::endl; }

        virtual void f() { std::cout << "C_BA::f()" << std::endl; }
    };


    void test1();
    void test2();
    void dump(uint8_t* data, uint32_t size);

    inline void test() {
        std::cout << "virtual functions tests" << std::endl;

        test1();
        //test2();
    }
}