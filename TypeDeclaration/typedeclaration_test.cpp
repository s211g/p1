#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <functional>
#include <memory>

#include "typedeclaration_test.hpp"

namespace typedeclaration_test {

    void test_array() {
        std::cout << "test array" << std::endl;
        int a1[10];
        int a2[10];
        swap_array(a1, a2);
    }

    void test_ptr_to_fn() {
        // ptr to fn

        // ptr to class member fn
    }
}