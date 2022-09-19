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


template <typename T>
class A {
public:
    int i;
};

template <typename T>
class B : public A<T> {
    using A<T>::i;

public:
    void f() {
        i = 0;
    }
};

int main() {
    B<int> b;

    //mutex_test::test();
    //smartpointer_test::test();
    //scopeguard_test::test();
    thread_test::test();
    //vatemplate_test::test();
    //typedeclaration_test::test();
    //initdata_test::test();
    //functional_hdr_test::test();
    //future_hdr_test::test();
    //chrono_hdr_test::test();
    //atomic_hdr_test::test();

    return 0;
}
