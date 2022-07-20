#pragma once

#include <iostream>

namespace smartpointer_test {

    void test1();

    inline void test() {
        std::cout << "smartpointer tests" << std::endl;
        test1();
    }
}