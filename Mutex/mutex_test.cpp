
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <functional>

#include "mutex_test.hpp"

namespace mutex_test {

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

                // ????????????
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


    void test_shared_mutex() {
        std::cout << "test std::shared_mutex" << std::endl;

        auto th_fn = [](std::string name, std::shared_mutex& mtx, int c) {
            std::cout << "Start thread " << name << std::endl
                      << std::flush;
            while (c--) {
                std::cout << "thread " << name << " try lock ..." << std::endl;
                {
                    std::lock_guard lock(mtx);
                    std::cout << "--------------- thread " << name << " lock, c = " << c << std::endl
                              << std::flush;
                    std::this_thread::sleep_for(std::chrono::seconds(1));
                }
                std::cout << "----------------- thread " << name << " unlock" << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            std::cout << "Stop thread " << name << std::endl
                      << std::flush;
        };

        auto th_fn_shared_lock = [](std::string name, std::shared_mutex& mtx, int interval_ms, bool& terminate) {
            std::cout << "Start thread " << name << std::endl
                      << std::flush;
            while (!terminate) {
                std::cout << "thread " << name << " try shared lock ..." << std::endl;
                {
                    std::shared_lock lock(mtx);
                    std::cout << "thread " << name << " shared lock" << std::endl
                              << std::flush;
                    std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
                }
                std::cout << "thread " << name << "shared unlock" << std::endl;
            }
            std::cout << "Stop thread " << name << std::endl
                      << std::flush;
        };

        bool terminate = false;

        std::shared_mutex mtx;
        std::thread th1 = std::thread(th_fn, "1", std::ref(mtx), 5);
        std::thread th2 = std::thread(th_fn_shared_lock, "2", std::ref(mtx), 200, std::ref(terminate));
        std::thread th3 = std::thread(th_fn_shared_lock, "3", std::ref(mtx), 50, std::ref(terminate));

        th1.join();
        terminate = true;
        th2.join();
        th3.join();
    }

    void test_multiple_lock() {
        auto th_fn = [](std::string name, std::mutex& mtx, int c, int lock_ms, int unlock_ms) {
            std::cout << "Start thread " << name << std::endl
                      << std::flush;
            while (c--) {
                std::cout << "thread " << name << " try lock ..." << std::endl;
                {
                    std::lock_guard lock(mtx);
                    std::cout << "thread " << name << " lock, c = " << c << std::endl
                              << std::flush;
                    std::this_thread::sleep_for(std::chrono::milliseconds(lock_ms));
                    std::cout << "thread " << name << " unlock" << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(unlock_ms));
            }
            std::cout << "Stop thread " << name << std::endl
                      << std::flush;
        };

        auto th_fn_multiple_lock = [](std::string name, std::mutex& mtx1, std::mutex& mtx2, int lock_ms, int unlock_ms, bool& terminate) {
            std::cout << "Start thread " << name << std::endl
                      << std::flush;
            while (!terminate) {
                std::cout << "thread " << name << " try lock ..." << std::endl;
                {
                    // equivalent approach 1:
                    // std::lock(mtx1, mtx2);
                    // std::adopt_lock - calling thread already has ownership of the mutex
                    //std::lock_guard lock1(mtx1, std::adopt_lock);
                    //std::lock_guard lock2(mtx2, std::adopt_lock);

                    // equivalent approach 2:
                    // defer_lock -	do not acquire ownership of the mutex
                    std::unique_lock lock1{mtx1, std::defer_lock};
                    std::unique_lock lock2{mtx2, std::defer_lock};
                    std::lock(lock1, lock2);
                    std::cout << "thread " << name << " lock" << std::endl;

                    std::this_thread::sleep_for(std::chrono::milliseconds(lock_ms));
                    std::cout << "thread " << name << " unlock" << std::endl;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(unlock_ms));
            }
            std::cout << "Stop thread " << name << std::endl
                      << std::flush;
        };

        bool terminate = false;
        std::mutex mtx1;
        std::mutex mtx2;
        std::thread th1 = std::thread(th_fn, "1", std::ref(mtx1), 20, 100, 100);
        std::thread th2 = std::thread(th_fn, "2", std::ref(mtx2), 20, 100, 100);
        std::thread th3 = std::thread(th_fn_multiple_lock, " ------------ 3", std::ref(mtx1), std::ref(mtx2), 2000, 2000, std::ref(terminate));

        th1.join();
        terminate = true;
        th2.join();
        th3.join();
    }
}