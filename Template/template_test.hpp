#pragma once

#include <iostream>

namespace template_test {

    void test_template_template_parameters();
    void test_enable_if();
    void test_friend();
    void test_template_parameters();

    inline void test() {
        std::cout << "template tests" << std::endl;

        test_template_template_parameters();
        //test_enable_if();
        //test_friend();
        //test_template_parameters();
    }
}