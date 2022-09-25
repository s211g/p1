#pragma once

#include <iostream>

namespace containers_test {

    void test_array();
    void test_vector();
    void test_set();
    void test_unordered_map();

    inline void test() {
        std::cout << "containers tests" << std::endl;
        //test_array();
        //test_vector();
        //test_set();
        test_unordered_map();
    }
}