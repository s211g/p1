
#include "rtti_cast_test.hpp"

namespace rtti_cast_test {

    class A {
        virtual void f(){};
    };

    class B : public A {};

    void
    test_type_info() {
        std::cout << "test typeid()" << std::endl;

        const auto& ti = typeid(A);
        std::cout << " typeid(A).name() : " << ti.name() << std::endl;
        std::cout << " typeid(B).name() : " << typeid(B).name() << std::endl;

        A a;
        std::cout << " typeid(a).name() : " << ti.name() << std::endl;

        // out:
        // typeid(A).name() : class rtti_cast_test::A
        // typeid(B).name() : class rtti_cast_test::B
        // typeid(a).name() : class rtti_cast_test::A
    }
}