#pragma once

#include <iostream>

namespace atomic_hdr_test {

    void test_bool();
    void test_ptr();

    inline void test() {
        std::cout << "atomic header tests" << std::endl;

        //test_bool();
        test_ptr();
    }
}
