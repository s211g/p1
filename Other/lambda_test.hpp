#pragma once

#include <iostream>

namespace lambda_test {

    void test_lambda_local_variables();
    void test_lambda_auto_arguments();
    void test_lambda_constexpr();
    void test_lambda_this();
    void test_lambda();


    inline void test() {
        std::cout << "lambda_test" << std::endl;

        //test_lambda_local_variables();
        //test_lambda_auto_arguments();
        //test_lambda_constexpr();
        test_lambda_this();
        //test_lambda();
    }
}