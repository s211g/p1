#pragma once

#include <iostream>

namespace patterns_structure_test {

    void test_adapter();

    inline void test() {
        std::cout << "patterns structure tests" << std::endl;

        test_adapter();
    }
}