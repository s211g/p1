#pragma once

#include <iostream>

namespace virtual_table_test {

    void test1();
    void test2();

    inline void test() {
        std::cout << "\nvirtual_table_test" << std::endl;

        //test1();
        test2();
    }
}