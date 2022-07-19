
#include <iostream>
#include <mutex>
#include <thread>
#include <functional>

#include "mutex_test.hpp"

namespace mutex_test {
    void test() {
        std::cout << "mutex_test()" << std::endl;
        //test_mutex();
        test_recursive_mutex();
    }

    void test_mutex() {
        std::cout << "test std::mutex" << std::endl;

        auto th_fn = [](std::string name, std::mutex& mtx, int c) {
            std::cout << "Start thread " << name << std::endl;
            while (c--) {
                //std::cout << "thread: " << name << " с = " << c << std::endl;
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    std::cout << "thread: " << name << " begin" << std::endl;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    std::cout << "thread: " << name << " end" << std::endl;
                }

                // ???????????
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
            }
        };

        std::mutex mtx;
        std::thread th1 = std::thread(th_fn, "1", std::ref(mtx), 10);
        std::thread th2 = std::thread(th_fn, "2", std::ref(mtx), 10);

        th2.join();
        th1.join();
    }

    void test_recursive_mutex() {
        std::cout << "test std::recursive_mutex" << std::endl;

        std::function<void(const std::string& name, std::recursive_mutex& mtx, int& c)> recursive_fn;
        recursive_fn = [&recursive_fn](const std::string& name, std::recursive_mutex& mtx, int& c) {
            std::lock_guard lock(mtx);
            std::cout << "recursive_fn() thread: " << name << " c = " << c << std::endl
                      << std::flush;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if (--c)
                recursive_fn(name, mtx, c);
        };

        auto th_fn = [&recursive_fn](std::string name, std::recursive_mutex& mtx, int c) {
            std::cout << "Start thread " << name << std::endl
                      << std::flush;
            recursive_fn(name, mtx, c);
        };

        std::recursive_mutex mtx;
        std::thread th1 = std::thread(th_fn, "1", std::ref(mtx), 10);
        std::thread th2 = std::thread(th_fn, "2", std::ref(mtx), 10);

        th2.join();
        th1.join();
    }



}