#pragma once

#include <iostream>

namespace smartpointer_test {

    void test_shared_ptr();
    void test_unique_ptr();
    void test_weak_ptr();
    inline void test() {
        std::cout << "smartpointer tests" << std::endl;
        ///test_shared_ptr();
        //test_unique_ptr();
        test_weak_ptr();
    }
}