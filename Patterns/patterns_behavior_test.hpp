#pragma once

#include <iostream>

namespace patterns_behavior_test {

    void test_chain_of_responsibility();

    inline void test() {
        std::cout << "patterns behavior tests" << std::endl;

        test_chain_of_responsibility();
    }
}