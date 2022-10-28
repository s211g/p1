#pragma once

#include <iostream>

namespace patterns_behavior_test {

    void test_chain_of_responsibility();
    void test_command();
    void test_interpreter();
    void test_iterator();
    void test_mediator();
    void test_memento();
    void test_observer();
    void test_state();
    void test_strategy();
    void test_template_method();
    void test_visitor();

    inline void test() {
        std::cout << "patterns behavior tests" << std::endl;

        //test_chain_of_responsibility();
        //test_command();
        //test_interpreter();
        //test_iterator();
        //test_mediator();
        //test_memento();
        //test_observer();
        //test_state();
        //test_strategy();
        //test_template_method();
        test_visitor();
    }
}