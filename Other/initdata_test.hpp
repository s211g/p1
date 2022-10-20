#pragma once

#include <iostream>

namespace initdata_test {

    void test_call_once();
    void test_lambda();

    inline void test() {
        std::cout << "init data tests" << std::endl;

        //test_call_once();
        test_lambda();
    }
}