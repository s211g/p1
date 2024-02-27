#pragma once

#include <iostream>

namespace delete_test {

    void test_delete_class();
    void test_delete_array();

    inline void test() {
        std::cout << "delete_test" << std::endl;

        //test_delete_class();
        test_delete_array();
    }
}