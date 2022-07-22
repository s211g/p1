#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <functional>
#include <memory>

#include "thread_test.hpp"
#include "ThreadsPool.hpp"

namespace thread_test {
    void test1() {
        int const ht = std::thread::hardware_concurrency();
        std::cout << "hardware_concurrency = " << ht << std::endl;
    }

    void test_thread_pool() {
        std::cout << "test thread pool" << std::endl;

        std::atomic_bool terminate{false};
        ThreadsPool::ThreadsPool pool(terminate);
        pool.start(10);

        auto fn = [](int i) {
            std::cout << "fn() i = " << i << " start ..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "fn() " << i << " complete" << std::endl;
        };

        for (int i = 0; i < 100; ++i) {
            auto task = std::bind(fn, i);
            pool.schedule_work(task);
        }

        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "pool terminate ----------------------------------------" << std::endl;
        terminate = true;
        pool.wait();
    }
}