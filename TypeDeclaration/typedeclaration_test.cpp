#include <iostream>
#include "typedeclaration_test.hpp"

namespace typedeclaration_test {

    void test_array() {
        std::cout << "test array" << std::endl;
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
        int fn(int i) {
            std::cout << "A::fn(" << i << ")" << std::endl;
            return 0;
        }
    };

    void test_ptr_to_fn() {
        std::cout << "test fn ptr" << std::endl;

        std::cout << "test pointer to fn" << std::endl;
        // ptr to fn
        // тип (*имя_указателя) (параметры);
        int (*p_fn)(int, double);
        p_fn = &fn1;
        p_fn(1, 2.3);
        (****p_fn)(1, 2.3);

        std::cout << "test pointer to fn[3]" << std::endl;
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
}