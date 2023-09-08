#pragma once

#include <iostream>

namespace constructor_test {

    void test1();

    inline void test() {
        std::cout << "constructor tests" << std::endl;

        test1();
    }
}
