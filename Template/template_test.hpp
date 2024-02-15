#pragma once

#include <iostream>

namespace template_test {

    void test1();
    void test2();
    void test_enable_if();
    void test_friend();

    inline void test() {
        std::cout << "template tests" << std::endl;

        //test1();
        test2();
        //test_enable_if();
        //test_friend();
    }
}