#pragma once

#include <iostream>

namespace template_test {

    void test_enable_if();
    void test_friend();

    inline void test() {
        std::cout << "template tests" << std::endl;

        //test_enable_if();
        //test_friend();
    }
}