#pragma once

#include <iostream>

namespace mutex_test {

    void test_mutex();
    void test_recursive_mutex();
    void test_timed_mutex();
    void test_shared_mutex();
    void test_multiple_lock();
    void test_call_once();

    inline void test() {
        std::cout << "mutex tests" << std::endl;
        //test_mutex();
        //test_recursive_mutex();
        //test_timed_mutex();
        //test_shared_mutex();
        //test_multiple_lock();
        test_call_once();
    }
}