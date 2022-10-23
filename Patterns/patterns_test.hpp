#pragma once

#include <iostream>

namespace patterns_test {

    void test_fabric();
    void test_abstract_factory();
    void test_builder();
    void test_singleton();
    void test_prototype();

    void test_adapter();

    inline void test() {
        std::cout << "patterns tests" << std::endl;

        //test_fabric();
        //test_abstract_factory();
        //test_builder();
        //test_singleton();
        //test_prototype();

        test_adapter();
    }
}