#pragma once

#include <iostream>

namespace chrono_hdr_test {

    void test_duration();
    void test_time_point();

    inline void test() {
        std::cout << "chrono header tests" << std::endl;

        //test_duration();
        test_time_point();
    }
}