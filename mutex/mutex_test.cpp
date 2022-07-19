
#include <iostream>
#include <mutex>
#include <thread>
#include <functional>

#include "mutex_test.hpp"

namespace mutex_test {
    void test() {
        std::cout << "mutex_test()" << std::endl;
        //test_mutex();
        //test_recursive_mutex();
        test_timed_mutex();
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

    void test_timed_mutex() {
        std::cout << "test std::timed_mutex" << std::endl;

        auto th_fn = [](std::string name, std::timed_mutex& mtx, int c) {
            std::cout << "Start thread " << name << std::endl
                      << std::flush;
            std::lock_guard lock(mtx);
            while (c--) {
                std::cout << "thread " << name << " c = " << c << std::endl
                          << std::flush;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            std::cout << "Stop thread " << name << std::endl
                      << std::flush;
        };

        std::timed_mutex mtx;
        int c = 5;

        {
            std::cout << "test try_lock() ------------------------------------------- " << std::endl;
            std::thread th1 = std::thread(th_fn, "1", std::ref(mtx), c);
            std::this_thread::sleep_for(std::chrono::seconds(1));

            int cc      = c + 3;
            bool locked = false;
            while (cc--) {
                std::cout << "main thread, try lock ... " << std::endl
                          << std::flush;
                bool lock = mtx.try_lock();
                locked |= lock;

                std::cout << "main thread, try lock " << (lock ? "true" : "false") << std::endl
                          << std::flush;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            if (locked)
                mtx.unlock();
            th1.join();
        }

        {
            std::cout << "test try_lock_until() ------------------------------------------- " << std::endl;
            auto th_fn_try_lock = [](std::string name, std::timed_mutex& mtx, int c) {
                std::cout << "test try_lock_until() thread " << name << " ... " << std::endl;
                std::chrono::time_point t = std::chrono::system_clock::now() + std::chrono::seconds(c);
                bool lock                 = mtx.try_lock_until(t);
                std::cout << "thread " << name << ", try lock " << (lock ? "true" : "false") << std::endl
                          << std::flush;
                if (lock)
                    mtx.unlock();
                std::cout << "test try_lock_until() thread " << name << " ... exit " << std::endl;
            };

            std::thread th1 = std::thread(th_fn, "1", std::ref(mtx), c);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::thread th2 = std::thread(th_fn_try_lock, "2", std::ref(mtx), c - 1);
            std::thread th3 = std::thread(th_fn_try_lock, "3", std::ref(mtx), c + 2);

            th1.join();
            th2.join();
            th3.join();
        }

        {
            std::cout << "test try_lock_for() ------------------------------------------- " << std::endl;
            auto th_fn_try_lock = [](std::string name, std::timed_mutex& mtx, int c) {
                std::cout << "test try_lock_for() thread " << name << " ... " << std::endl;
                bool lock = mtx.try_lock_for(std::chrono::seconds(c));
                std::cout << "thread " << name << ", try lock " << (lock ? "true" : "false") << std::endl
                          << std::flush;
                if (lock)
                    mtx.unlock();
                std::cout << "test try_lock_for() thread " << name << " ... exit " << std::endl;
            };

            std::thread th1 = std::thread(th_fn, "1", std::ref(mtx), c);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::thread th2 = std::thread(th_fn_try_lock, "2", std::ref(mtx), c - 1);
            std::thread th3 = std::thread(th_fn_try_lock, "3", std::ref(mtx), c + 2);

            th1.join();
            th2.join();
            th3.join();
        }
    }
}