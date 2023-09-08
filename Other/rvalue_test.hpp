#pragma once

#include <iostream>

namespace rvalue_test {

    void test1();
    void test2();
    void test3();

    inline void test() {
        std::cout << "rvalue tests" << std::endl;

        //test1();
        test2();
        // test3();
    }
}
