#pragma once

#include <iostream>

namespace template_lambda_test {

    void test1();
    void test_lambdas_collections();

    inline void test() {
        std::cout << "template_lambda_test" << std::endl;

        //test1();
        test_lambdas_collections();
    }
}