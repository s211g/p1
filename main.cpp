#include <iostream>
#include <memory>
#include <functional>

#include "mutex_test.hpp"
#include "smartpointer_test.hpp"
#include "scopeguard_test.hpp"
#include "thread_test.hpp"
#include "vatemplate_test.hpp"
#include "typedeclaration_test.hpp"
#include "initdata_test.hpp"
#include "functional_hdr_test.hpp"
#include "future_hdr_test.hpp"
#include "chrono_hdr_test.hpp"
#include "atomic_hdr_test.hpp"
#include "containers_test.hpp"
#include "crtp_test.hpp"
#include "template_test.hpp"

int main() {
    //containers_test::test();
    //mutex_test::test();
    //smartpointer_test::test();
    //scopeguard_test::test();
    //thread_test::test();
    //vatemplate_test::test();
    //typedeclaration_test::test();
    //initdata_test::test();
    //functional_hdr_test::test();
    //future_hdr_test::test();
    //chrono_hdr_test::test();
    //atomic_hdr_test::test();
    //crtp_test::test();
    template_test::test();

    return 0;
}
