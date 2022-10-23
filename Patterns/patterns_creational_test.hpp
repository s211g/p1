#pragma once

#include <iostream>

namespace patterns_creational_test {

    void test_fabric();
    void test_abstract_factory();
    void test_builder();
    void test_singleton();
    void test_prototype();

    inline void test() {
        std::cout << "patterns creational tests" << std::endl;

        //test_fabric();
        //test_abstract_factory();
        //test_builder();
        //test_singleton();
        test_prototype();
    }
}