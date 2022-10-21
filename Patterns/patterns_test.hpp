#pragma once

#include <iostream>

namespace patterns_test {

    void test_fabric();

    inline void test() {
        std::cout << "patterns tests" << std::endl;
        test_fabric();
    }
}