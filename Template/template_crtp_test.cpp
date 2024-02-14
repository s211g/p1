#include <iostream>
#include <functional>
#include "template_crtp_test.hpp"

namespace template_crtp_test {

    void test_1() {
        std::cout << "\ntest crtp" << std::endl;
        A a1{11};
        A a2{12};
        A a3{3};
        a3 = a1 + a2;
        std::cout << "a3.i = " << a3.i << std::endl;

        A2 a21{11};
        A2 a22{12};
        A2 a23{3};
        a23 = a21 + a22;
        std::cout << "a23.i = " << a23.i << std::endl;
    }

}