#pragma once

#include <iostream>

namespace typedeclaration_test {

    void test_array();
    template <typename T, size_t N>
    void swap_array(T (&a1)[N], T (&a2)[N]) {
        std::cout << "swap_arra()" << std::endl;
    }

    void test_ptr_to_fn();

    inline void test() {
        std::cout << "typedeclaration tests" << std::endl;
        test_array();
        test_ptr_to_fn();
    }
}