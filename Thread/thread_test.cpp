#include <iostream>

#include "thread_test.hpp"
#include "ThreadsPool.hpp"
#include "ThreadSafeQueue.hpp"

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

    void test_thread_safe_queue() {
        std::cout << "\ntest thread safe queue" << std::endl;

        typedef ThreadSafeQueue::ThreadSafeQueue1<int> qInt_t;
        qInt_t q;

        auto th_populate = [](qInt_t& q, std::string th_name, int start, int count, int interval_ms) {
            std::cout << th_name << " ... start" << std::endl;
            int i = 0;
            while (i < count) {
                q.push(start + i++);
                std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
            }
            std::cout << th_name << " ... stop" << std::endl;
            return count;
        };

        auto th_read = [](qInt_t& q, std::string th_name, uint32_t wait_ms, std::atomic_bool& terminate) {
            std::cout << th_name << "... start" << std::endl;
            int i     = 0;
            int count = 0;
            while (!terminate) {
                int value;


                if (wait_ms) {
                    if (q.pop(value, wait_ms)) {
                        std::cout << th_name << " read : " << value << std::endl;
                        ++count;
                    }
                }
                else {
                    if (q.pop(value)) {
                        std::cout << th_name << "----------- read : " << value << std::endl;
                        ++count;
                    }
                }

                if (q.empty()) {
                    std::cout << th_name << " queue is empty" << std::endl;
                    if (wait_ms)
                        std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
                    else
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
            std::cout << th_name << "... stop, count = " << count << std::endl;
            return count;
        };

        std::atomic_bool terminate = false;
        int iter_count             = 200;
        auto p1                    = std::async(th_populate, std::ref(q), "populate1", 1000, iter_count, 10);
        auto p2                    = std::async(th_populate, std::ref(q), "populate2", 2000, iter_count, 10);
        auto r1                    = std::async(th_read, std::ref(q), "read1", 10, std::ref(terminate));
        auto r2                    = std::async(th_read, std::ref(q), "read2", 10, std::ref(terminate));
        auto r3                    = std::async(th_read, std::ref(q), "read3", 10, std::ref(terminate));
        auto r4                    = std::async(th_read, std::ref(q), "read4", 0, std::ref(terminate));

        int populate_total = 0;
        int read_total     = 0;

        populate_total += p1.get();
        populate_total += p2.get();
        terminate = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        read_total += r1.get();
        read_total += r2.get();
        read_total += r3.get();
        read_total += r4.get();
        std::cout << "populate_total = " << populate_total << " read_total = " << read_total << std::endl;
    }
}
