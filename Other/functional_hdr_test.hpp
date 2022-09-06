#pragma once

#include <iostream>

namespace functional_hdr_test {

    void test_bind();
    void test_function();
    void test_memfn();

    inline void test() {
        std::cout << "functional header tests" << std::endl;

        test_bind();
        test_function();
        test_memfn();
    }
}