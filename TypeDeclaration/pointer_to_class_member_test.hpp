#pragma once

#include <iostream>

namespace pointer_to_class_member_test {

    void test1();

    inline void test() {
        std::cout << "pointer_to_class_member_test" << std::endl;

        test1();
    }
}