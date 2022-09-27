#pragma once

#include <iostream>

namespace containers_test {

    void test_array();
    void test_vector();
    void test_set();
    void test_unordered_map();
    void test_tuple();
    void test_variant();
    void test_optional();

    inline void test() {
        std::cout << "containers tests" << std::endl;
        //test_array();
        //test_vector();
        //test_set();
        //test_unordered_map();
        //test_tuple();
        //test_variant();
        test_optional();
    }
}