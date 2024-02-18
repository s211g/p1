#pragma once

#include <iostream>

namespace virtual_table_test {

    void test1();

    inline void test() {
        std::cout << "\nvirtual_table_test" << std::endl;

        test1();
    }
}