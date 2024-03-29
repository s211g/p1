#include <iostream>

// Containers
#include "containers_test.hpp"

// Inheritance
#include "inheritance_test.hpp"

// Mutex
#include "mutex_test.hpp"

// Other
#include "atomic_hdr_test.hpp"
#include "chrono_hdr_test.hpp"
#include "constructor_test.hpp"
#include "functional_hdr_test.hpp"
#include "future_hdr_test.hpp"
#include "lambda_test.hpp"
#include "rtti_cast_test.hpp"
#include "rvalue_test.hpp"
#include "static_test.hpp"
#include "delete_test.hpp"

// Patterns
#include "patterns_creational_test.hpp"
#include "patterns_structure_test.hpp"
#include "patterns_behavior_test.hpp"

// ScopeGuard
#include "scopeguard_test.hpp"

// SmartPointer
#include "smartpointer_test.hpp"

// Template
#include "template_explicit_specialization_test.hpp"
#include "template_template_specialization_test.hpp"
#include "template_test.hpp"
#include "template_lambda_test.hpp"
#include "template_crtp_test.hpp"
#include "template_universal_arguments_test.hpp"
#include "template_variable_arguments_test.hpp"

// Thread
#include "thread_test.hpp"

// TypeDeclaration
#include "typedeclaration_test.hpp"
#include "pointer_to_class_member_test.hpp"

// Virtual
#include "virtual_function_test.hpp"
#include "virtual_table_test.hpp"


int main() {
    std::cout << "sizeof(void*) : " << sizeof(void*) << std::endl
              << std::endl;

    // Containers
    // containers_test::test();

    // Inheritance
    // inheritance_test::test();

    // Mutex
    // mutex_test::test();

    // Other
    // atomic_hdr_test::test();
    // chrono_hdr_test::test();
    // constructor_test::test();
    // functional_hdr_test::test();
    // future_hdr_test::test();
    // lambda_test::test();
    // rtti_cast_test::test();
    // rvalue_test::test();
    static_test::test();
    // delete_test::test();

    // Patterns
    // patterns_creational_test::test();
    // patterns_structure_test::test();
    // patterns_behavior_test::test();

    // ScopeGuard
    // scopeguard_test::test();

    // SmartPointer
    // smartpointer_test::test();

    // Template
    // template_explicit_specialization_test::test();
    // template_template_specialization_test::test();
    // template_test::test();
    // template_lambda_test::test();
    // template_crtp_test::test();
    // template_universal_arguments_test::test();
    // template_variable_arguments_test::test();

    // Thread
    // thread_test::test();

    // TypeDeclaration
    // typedeclaration_test::test();
    // pointer_to_class_member_test::test();

    // Virtual
    //virtual_function_test::test();
    //virtual_table_test::test();

    return 0;
}
