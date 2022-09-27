#include <iostream>
#include <functional>
#include "typedeclaration_test.hpp"

namespace typedeclaration_test {

    void test_array() {
        std::cout << "\ntest array" << std::endl;
        int a1[10];
        int a2[10];
        swap_array(a1, a2);
    }

    int fn1(int i, double d) {
        std::cout << "fn1() " << i << " " << d << std::endl;
        return 0;
    }
    int fn2(int i, double d) {
        std::cout << "fn2() " << i << " " << d << std::endl;
        return 0;
    }
    int fn3(int i, double d) {
        std::cout << "fn3() " << i << " " << d << std::endl;
        return 0;
    }

    class A {
    public:
        A() { std::cout << "A::A()" << std::endl; }
        A(int i) { std::cout << "A::A(int)" << std::endl; }
        int fn(int i) {
            std::cout << "A::fn(" << i << ")" << std::endl;
            return 0;
        }
    };

    void test_ptr_to_fn() {
        std::cout << "\ntest fn ptr" << std::endl;

        std::cout << "\ntest pointer to fn" << std::endl;
        // ptr to fn
        // тип (*имя_указателя) (параметры);
        int (*p_fn)(int, double);
        p_fn = &fn1;
        p_fn(1, 2.3);
        (****p_fn)(1, 2.3);

        std::cout << "\ntest pointer to fn[3]" << std::endl;
        // ptr to array fn[]
        // тип (*имя_указателя[size]) (параметры);
        int (*p_fnarr[3])(int, double) = {fn1, fn2, fn3};
        p_fnarr[0](1, 1.1);
        p_fnarr[1](2, 2.2);
        p_fnarr[2](3, 3.3);

        // ptr to class member fn
        // retval (ClassName::*PtrName) (Parmeters);
        //int (A::*p_A_fn)(int) = A::fn;  - ERROR
        int (A::*p_A_fn)(int) = &A::fn;
        A a;
        (a.*p_A_fn)(1);
        (a.*p_A_fn)(1);
    }

    A a();
    A a(int);

    void test_declaration_fn() {
        std::cout << "\ntest declaration fn" << std::endl;
        a();
        a(1);
    }

    A a() {
        std::cout << "call A a()" << std::endl;
        return A{};
    }
    A a(int) {
        std::cout << "call A a(int)" << std::endl;
        return A{};
    }

    int* f(int) {
        std::cout << "f()" << std::endl;
        return (int*)nullptr;
    }

    void test_declaration_lambda() {
        std::cout << "\ntest make lambda" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        auto make_lambda = [](int param) { return [param](int value) { return value + param; }; };
        auto l1          = make_lambda(1);
        auto l2          = make_lambda(2);
        std::cout << l1(1) << std::endl;
        std::cout << l2(1) << std::endl;

        std::cout << "\ntest 2 internal state lambda" << std::endl;
        int i = 0;
        // mutable разрешает менять внутриние переменные переданые в кв скобках как копии
        auto l11 = [i]() mutable { std::cout << ++i << std::endl; };
        l11();
        l11();
        std::cout << "i = " << i << std::endl;
        auto l12 = [&i]() { std::cout << ++i << std::endl; };
        l12();
        l12();
        std::cout << "i = " << i << std::endl;

        std::cout << "\ntest 3" << std::endl;
        // объявляется как
        // int*(int)
        // а кастица как
        // int* (*)(int)
        std::function<int*(int)> l3 = f;
        l3(1);
        int* (*pl3)(int) = *l3.target<int* (*)(int)>();
        if (!pl3)
            std::cout << "pl3 == nullptr" << std::endl;
        else
            (*pl3)(0);

        std::cout << "\ntest 4" << std::endl;
    }
}