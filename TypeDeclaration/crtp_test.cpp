#include <iostream>
#include <functional>
#include "crtp_test.hpp"

namespace crtp_test {

    void test_1() {
        std::cout << "\ntest crtp" << std::endl;
        A a1{11};
        A a2{12};
        A a3{3};
        a3 = a1 + a2;
        std::cout << "a3.i = " << a3.i << std::endl;
    }


}