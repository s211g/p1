#pragma once

#include <iostream>

namespace template_template_specialization_test {

    void test_template_template_parameters();
    void test_template_parameters();

    inline void test() {
        std::cout << "template_template_specialization_test" << std::endl;

        //test_template_template_parameters();
        test_template_parameters();
    }
}