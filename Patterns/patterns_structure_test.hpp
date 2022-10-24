#pragma once

#include <iostream>

namespace patterns_structure_test {

    void test_adapter();
    void test_bridge();
    void test_composite();

    inline void test() {
        std::cout << "patterns structure tests" << std::endl;

        //test_adapter();
        //test_bridge();
        test_composite();
    }
}