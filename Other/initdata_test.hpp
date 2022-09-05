#pragma once

#include <iostream>

namespace initdata_test {

    void test_call_once();

    inline void test() {
        std::cout << "init data tests" << std::endl;

        test_call_once();
    }
}