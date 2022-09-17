#pragma once

#include <iostream>

namespace thread_test {

    void test1();
    void test_thread_pool();
    void test_thread_safe_queue();
    void test_LockFreeStack();
    void test_LockFreeStackS();
    void test_ThreadSafeLTable();

    inline void test() {
        std::cout << "thread tests" << std::endl;

        //test1();
        //test_thread_pool();
        //test_thread_safe_queue();
        //test_LockFreeStack();
        //test_LockFreeStackS();
        test_ThreadSafeLTable();
    }
}