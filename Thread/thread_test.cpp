#include <iostream>
#include <string>
#include <cassert>

#include "thread_test.hpp"
#include "ThreadsPool.hpp"
#include "ThreadSafeQueue.hpp"
#include "ThreadSafeStack.hpp"
#include "ThreadSafeLTable.hpp"
#include "ThreadSafeList.hpp"

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
        //std::cout << "\ntest ThreadSafeQueue::ThreadSafeQueue" << std::endl;
        //typedef ThreadSafeQueue::ThreadSafeQueue<int> qInt_t;

        std::cout << "\ntest ThreadSafeQueue::ThreadSafeQueueS" << std::endl;
        typedef ThreadSafeQueue::ThreadSafeQueueS<int> qInt_t;

        qInt_t q;

        auto th_populate = [](qInt_t& q, std::string th_name, int start, int count, int interval_ms) {
            std::cout << th_name << " ... start" << std::endl;
            int i = 0;
            while (i < count) {
                int value = start + i++;
                q.push(value);
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
                int value{0};

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

    void test_LockFreeStack() {
        std::cout << "\ntest ThreadSafeStack::LockFreeStack" << std::endl;

        typedef ThreadSafeStack::LockFreeStack<int> sInt_t;
        sInt_t q;

        auto th_populate = [](sInt_t& q, std::string th_name, int start, int count, int interval_ms) {
            std::cout << th_name << " ... start" << std::endl;
            int i = 0;
            while (i < count) {
                q.push(start + i++);
                std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
            }
            std::cout << th_name << " ... stop" << std::endl;
            return count;
        };

        auto th_read = [](sInt_t& q, std::string th_name, uint32_t wait_ms, std::atomic_bool& terminate) {
            std::cout << th_name << "... start" << std::endl;
            int i     = 0;
            int count = 0;
            while (!terminate) {
                int value;

                if (q.pop(value)) {
                    std::cout << th_name << "----------- read : " << value << std::endl;
                    ++count;
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
        std::cout << "LockFreeStack populate_total = " << populate_total << " read_total = " << read_total << std::endl;
    }

    void test_LockFreeStackS() {
        std::cout << "\ntest ThreadSafeStack::LockFreeStackS" << std::endl;

        struct StackData {
            int i;
        };

        typedef ThreadSafeStack::LockFreeStackS<StackData> sInt_t;
        sInt_t q;

        auto th_populate = [](sInt_t& q, std::string th_name, int start, int count, int interval_ms) {
            std::cout << th_name << " ... start" << std::endl;
            int i = 0;
            while (i < count) {
                StackData sd;
                sd.i = start + i++;
                q.push(sd);
                std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
            }
            std::cout << th_name << " ... stop" << std::endl;
            return count;
        };

        auto th_read = [](sInt_t& q, std::string th_name, uint32_t wait_ms, std::atomic_bool& terminate) {
            std::cout << th_name << "... start" << std::endl;
            int i     = 0;
            int count = 0;
            while (!terminate) {
                auto value = q.popS();
                if (value) {
                    std::cout
                        << th_name << "----------- read : " << value->i << std::endl;
                    ++count;
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
        std::cout << "LockFreeStackS populate_total = " << populate_total << " read_total = " << read_total << std::endl;
    }

    void test_ThreadSafeLTable() {
        std::cout << "\ntest ThreadSafeLTable::ThreadSafeLTable" << std::endl;

        typedef ThreadSafeLTable::ThreadSafeLTable<std::string, int> table_t;
        //table_t t;
        table_t t(1000000);
        int iter_count = 1000000;

        auto th_populate = [](table_t& t, std::string th_name, int start, int count, int interval_ms) {
            std::cout << th_name << " ... start" << std::endl;
            int add_count = 0;
            int i         = 0;
            while (i < count) {
                int value       = i++ + start;
                std::string key = std::to_string(value);
                if (t.addValue(key, value))
                    ++add_count;
                std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
            }
            std::cout << th_name << " ... stop" << std::endl;
            return add_count;
        };

        auto th_read = [](table_t& t, std::string th_name, int count, uint32_t wait_ms, std::atomic_bool& terminate) {
            std::cout << th_name << "... start" << std::endl;
            int read_count = 0;
            int i          = 0;
            //            while (i < count) {
            //                int value       = i++;
            while (count) {
                int value       = --count;
                std::string key = std::to_string(value);
                int value_      = 0;
                if (t.getValue(key, value_)) {
                    ++read_count;
                    assert(value == value_);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
            }
            std::cout << th_name << " ... stop" << std::endl;
            return read_count;
        };


        std::atomic_bool terminate = false;
        auto p1                    = std::async(th_populate, std::ref(t), "populate1", 0, iter_count, 0);
        auto p2                    = std::async(th_populate, std::ref(t), "populate2", 1000, iter_count, 0);
        auto r1                    = std::async(th_read, std::ref(t), "read1", iter_count + 1000, 0, std::ref(terminate));
        auto r2                    = std::async(th_read, std::ref(t), "read2", iter_count + 1000, 0, std::ref(terminate));
        auto r3                    = std::async(th_read, std::ref(t), "read3", iter_count + 1000, 0, std::ref(terminate));
        auto r4                    = std::async(th_read, std::ref(t), "read4", iter_count + 1000, 0, std::ref(terminate));

        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        int a1 = p1.get();
        int a2 = p2.get();
        std::cout << "LockFreeStackS populate p1 = " << a1 << std::endl;
        std::cout << "LockFreeStackS populate p2 = " << a2 << std::endl;
        std::cout << "LockFreeStackS populate total = " << a1 + a2 << std::endl;

        std::cout << "LockFreeStackS read r1 = " << r1.get() << std::endl;
        std::cout << "LockFreeStackS read r2 = " << r2.get() << std::endl;
        std::cout << "LockFreeStackS read r3 = " << r3.get() << std::endl;
        std::cout << "LockFreeStackS read r4 = " << r4.get() << std::endl;

        auto r5 = std::async(th_read, std::ref(t), "read5", iter_count + 1000, 0, std::ref(terminate));
        auto r6 = std::async(th_read, std::ref(t), "read6", iter_count + 1000, 0, std::ref(terminate));
        int rr5 = r5.get();
        int rr6 = r6.get();
        std::cout << "LockFreeStackS read r5 = " << rr5 << std::endl;
        std::cout << "LockFreeStackS read r6 = " << rr6 << std::endl;
    }

    void test_ThreadSafeList() {
        std::cout << "\ntest ThreadSafeList::ThreadSafeList" << std::endl;

        typedef ThreadSafeList::ThreadSafeList<std::string> table_t;
        table_t t;
        int iter_count = 1000;

        auto th_populate = [](table_t& t, std::string th_name, int start, int count, int interval_ms) {
            std::cout << th_name << " ... start" << std::endl;
            int add_count = 0;
            int i         = 0;
            while (i < count) {
                int value_        = i++ + start;
                std::string value = std::to_string(value_);
                t.push_front(value);
                ++add_count;
                std::this_thread::sleep_for(std::chrono::milliseconds(interval_ms));
            }
            std::cout << th_name << " ... stop" << std::endl;
            return add_count;
        };

        auto th_remove_if = [](table_t& t, std::string th_name, int count, uint32_t wait_ms, std::atomic_bool& terminate) {
            std::cout << th_name << "... start" << std::endl;
            int remove_count = 0;
            int i            = 0;
            while (i != count) {
                int value_ = i;
                i += 2;
                std::string value = std::to_string(value_);
                remove_count += t.remove_if([](const std::string& v) { return true; });
                std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
            }
            std::cout << th_name << " ... stop" << std::endl;
            return remove_count;
        };

        std::atomic_bool terminate = false;
        auto p1                    = std::async(th_populate, std::ref(t), "populate1", 0, iter_count, 0);
        auto p2                    = std::async(th_populate, std::ref(t), "populate2", 1000, iter_count, 0);
        auto r1                    = std::async(th_remove_if, std::ref(t), "remove", iter_count + 1000, 0, std::ref(terminate));
        auto r2                    = std::async(th_remove_if, std::ref(t), "remove", iter_count + 1000, 0, std::ref(terminate));

        int a1      = p1.get();
        int a2      = p2.get();
        int p_total = a1 + a2;
        std::cout << "populate p1 = " << a1 << std::endl;
        std::cout << "populate p2 = " << a2 << std::endl;
        std::cout << "populate total = " << p_total << std::endl;

        int rr1     = r1.get();
        int rr2     = r2.get();
        int r_total = rr1 + rr2;
        std::cout << "remove r1 = " << rr1 << std::endl;
        std::cout << "remove r2 = " << rr2 << std::endl;
        std::cout << "remove total = " << r_total << std::endl;

        std::cout << "table size = " << t.size() << ", pop-remove = " << p_total - r_total << std::endl;

        t.remove_if([](const std::string& value) { static int i{1}; return i++>10; });
        t.for_each([](const std::string& value) { static int i{1}; std::cout << "[" << i++ <<  "] : " << value << std::endl; });
        t.for_each([](std::string& value) { value += "__"; });
        t.for_each([](const std::string& value) { static int i{1}; std::cout << "[" << i++ <<  "] : " << value << std::endl; });
    }
}