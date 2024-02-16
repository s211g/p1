#pragma once

#include <iostream>

namespace template_lambda_test {

    void test1();
    void test2();

    inline void test() {
        std::cout << "template_lambda_test" << std::endl;

        test1();
        //test2();
    }
}