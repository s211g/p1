#pragma once

#include <iostream>

namespace thread_test {

    void test1();
    void test_thread_pool();
    void test_thread_safe_queue();
    void test_LockFreeQueueS1W1R();
    void test_LockFreeStack();
    void test_LockFreeStackXW1R();
    void test_LockFreeStackS();
    void test_LockFreeStackSXW1R();
    void test_ThreadSafeLTable();
    void test_ThreadSafeList();

    inline void test() {
        std::cout << "thread tests" << std::endl;

        //test1();
        //test_thread_pool();
        //test_thread_safe_queue();
        test_LockFreeQueueS1W1R();
        //test_LockFreeStackXW1R();
        //test_LockFreeStackSXW1R();
        //test_LockFreeStack();
        //test_LockFreeStackS();
        //test_ThreadSafeLTable();
        //test_ThreadSafeList();
    }
}