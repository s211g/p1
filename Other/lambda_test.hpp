#pragma once

#include <iostream>

namespace lambda_test {

    void test_lambda_local_variables();
    void test_lambda();

    inline void test() {
        std::cout << "lambda_test" << std::endl;

        test_lambda_local_variables();
        //test_lambda();
    }
}