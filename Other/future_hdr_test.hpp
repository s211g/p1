#pragma once

#include <iostream>

namespace future_hdr_test {

    void test_async();
    void test_packaged_task();
    void test_promise();

    inline void test() {
        std::cout << "future header tests" << std::endl;

        // test_async();
        // test_packaged_task();
        test_promise();
    }
}