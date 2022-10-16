#pragma once

#include <iostream>

namespace typedeclaration_test {

    void test_array();
    template <typename T, size_t N>
    void swap_array(T (&a1)[N], T (&a2)[N]) {
        std::cout << "swap_arra()" << std::endl;
    }

    void test_ptr_to_fn();
    void test_declaration_fn();
    void test_declaration_lambda();
    void test_type_inference();
    void test_auto();
    void test_bracket();
    void test_using();

    inline void test() {
        std::cout << "typedeclaration tests" << std::endl;
        //test_array();
        //test_ptr_to_fn();
        //test_declaration_fn();
        //test_declaration_lambda();
        //test_type_inference();
        //test_auto();
        //test_bracket();
        test_using();
    }
}