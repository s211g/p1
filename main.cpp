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
#include "template_specialization_test.hpp"

#include "patterns_creational_test.hpp"
#include "patterns_structure_test.hpp"
#include "patterns_behavior_test.hpp"
#include "virtual_function_test.hpp"
#include "rtti_cast_test.hpp"
#include "rvalue_test.hpp"
#include "constructor_test.hpp"
#include "static_test.hpp"
#include "inheritance_test.hpp"

int main() {
    std::cout << "sizeof(void*) : " << sizeof(void*) << std::endl
              << std::endl;

    //constructor_test::test();
    //rvalue_test::test();

    virtual_function_test::test();

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

    //template_test::test();
    //template_specialization_test::test();

    //patterns_creational_test::test();
    //patterns_structure_test::test();
    //patterns_behavior_test::test();

    //rtti_cast_test::test();
    //static_test::test();

    //inheritance_test::test();

    return 0;
}
