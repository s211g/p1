#pragma once

#include <string>
#include <iostream>
#include <ostream>


namespace template_universal_arguments_test {

    void test_template_argument();
    void test_universal_reference_argument();

    inline void test() {
        std::cout << "template_universal_arguments_test" << std::endl;

        test_template_argument();
        test_universal_reference_argument();
    }
}