#pragma once

#include <iostream>

namespace atomic_hdr_test {

    void test_bool();

    inline void test() {
        std::cout << "atomic header tests" << std::endl;

        test_bool();
    }
}
