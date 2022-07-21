#pragma once

#include <iostream>

namespace thread_test {

    void test_thread_pool();

    inline void test() {
        std::cout << "thread tests" << std::endl;

        test_thread_pool();
    }
}