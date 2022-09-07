#pragma once

#include <iostream>

namespace future_hdr_test {

    void test_async();
    void test_packaged_task();

    inline void test() {
        std::cout << "future header tests" << std::endl;

        //test_async();
        test_packaged_task();
    }
}