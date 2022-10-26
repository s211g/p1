#pragma once

#include <iostream>

namespace patterns_behavior_test {

    void test_chain_of_responsibility();
    void test_command();
    void test_interpreter();

    inline void test() {
        std::cout << "patterns behavior tests" << std::endl;

        //test_chain_of_responsibility();
        //test_command();
        test_interpreter();
    }
}