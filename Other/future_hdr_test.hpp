#pragma once

#include <iostream>

namespace future_hdr_test {

    void test_async();

    inline void test() {
        std::cout << "future header tests" << std::endl;

        test_async();
    }
}