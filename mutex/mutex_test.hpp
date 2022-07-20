#pragma once

namespace mutex_test {

    void test_mutex();
    void test_recursive_mutex();
    void test_timed_mutex();
    void test_shared_mutex();
    void test_multiple_lock();

    inline void test() {
        std::cout << "mutex_test()" << std::endl;
        //test_mutex();
        //test_recursive_mutex();
        //test_timed_mutex();
        //test_shared_mutex();
        test_multiple_lock();
    }
}