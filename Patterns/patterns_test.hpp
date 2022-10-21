#pragma once

#include <iostream>

namespace patterns_test {

    void test_fabric();
    void test_abstract_factory();

    inline void test() {
        std::cout << "patterns tests" << std::endl;
        //test_fabric();
        test_abstract_factory();
    }
}