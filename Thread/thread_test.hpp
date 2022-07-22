#pragma once

#include <iostream>

namespace thread_test {

    void test1();
    void test_thread_pool();

    inline void test() {
        std::cout << "thread tests" << std::endl;

        test1();
        test_thread_pool();
    }
}