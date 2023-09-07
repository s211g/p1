#pragma once

#include <iostream>

namespace rvalue_test {

    void test1();

    inline void test() {
        std::cout << "rvalue tests" << std::endl;

        test1();
    }
}
