#include <iostream>
#include <memory>
#include <functional>

#include "mutex_test.hpp"
#include "smartpointer_test.hpp"
#include "scopeguard_test.hpp"
#include "thread_test.hpp"
#include "vatemplate_test.hpp"
#include "typedeclaration_test.hpp"

int main() {
    //mutex_test::test();
    //smartpointer_test::test();
    //scopeguard_test::test();
    //thread_test::test();
    vatemplate_test::test();
    //typedeclaration_test::test();

    return 0;
}
