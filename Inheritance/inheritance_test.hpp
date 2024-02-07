#pragma once

#include <iostream>

namespace inheritance_test {

    void test_1();
    void test_2();
    void test_3();
    void test_4();

    inline void test() {
        std::cout << "inheritance tests" << std::endl;
        //test_1();
        //test_2();
        //test_3();
        test_4();
    }
}