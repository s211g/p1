#pragma once

#include <iostream>

namespace scopeguard_test {

    inline void test() {
        std::cout << "scope guard tests" << std::endl;
        // idiom p 231
    }
}