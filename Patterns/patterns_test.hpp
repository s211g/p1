#pragma once

#include <iostream>

namespace patterns_test {

    void test_fabric();
    void test_abstract_factory();
    void test_builder();
    void test_singleton();

    inline void test() {
        std::cout << "patterns tests" << std::endl;
        //test_fabric();
        //test_abstract_factory();
        //test_builder();
        test_singleton();
    }
}