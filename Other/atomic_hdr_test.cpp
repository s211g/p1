#include "atomic_hdr_test.hpp"
#include <atomic>
#include <string>

namespace atomic_hdr_test {

    void test_bool() {
        std::cout << "test std::atomic_bool " << std::endl;

        std::cout << "\ntest 1" << std::endl;
        //using atomic_bool = atomic<bool>;
        std::atomic_bool ab(false);

        //Checks whether the atomic operations on all objects of this type are lock-free
        std::cout << "ab.is_lock_free() : " << ab.is_lock_free() << std::endl;
        std::cout << "ab.is_always_lock_free : " << ab.is_always_lock_free << std::endl;

        std::cout << "value : " << (ab = true) << std::endl;
        std::cout << "value : " << ab.load() << std::endl;
        ab.store(false); // return void
        std::cout << "value : " << ab.load() << std::endl;
        std::cout << "set true, old value : " << ab.exchange(true) << std::endl;

        std::cout << "\ntest 2" << std::endl;
        ab             = false;
        bool expected  = true;
        bool new_value = true;
        std::cout << "current : " << ab << ", expected : " << expected << ", set new: " << new_value
                  << ", success : " << ab.compare_exchange_strong(expected, new_value) << ", result value: " << ab << std::endl;
        expected  = false;
        new_value = true;
        std::cout << "current : " << ab << ", expected : " << expected << ", set new: " << new_value
                  << ", success : " << ab.compare_exchange_strong(expected, new_value) << ", result value: " << ab << std::endl;

        std::cout << "\ntest 3" << std::endl;
        // compare_exchange_weak() - replace operation may fail
        // to set a new value to 100%, the operation is called in a loop
        expected  = true;
        new_value = false;
        std::cout << "current : " << ab << ", expected : " << expected << ", set new: " << new_value
                  << ", success : " << ab.compare_exchange_weak(expected, new_value) << ", result value: " << ab << std::endl;
    }
}