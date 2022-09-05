#pragma once

#include <iostream>

namespace functional_hdr_test {

    void test_bind();
    void test_function();

    inline void test() {
        std::cout << "functional header tests" << std::endl;

        test_bind();
        test_function();
    }
}