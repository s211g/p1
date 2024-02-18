#include "virtual_function_test.hpp"
#include "Utils.hpp"

namespace virtual_table_test {

    class Parent {
    public:
        long long i{1};
        virtual void Foo() {}
        virtual void FooNotOverridden() {}
    };

    class Derived : public Parent {
    public:
        long long j{2};
        void Foo() override {}
    };

    void test1() {
        std::cout << "\ntest1" << std::endl;


        Parent* pp  = new Parent();
        Derived* pd = new Derived();


        utils::dump("Parent", pp, sizeof(Parent), 8, true);
        utils::dump("Derived", pd, sizeof(Derived), 8, true);

        uint64_t vtable_parent  = *(reinterpret_cast<uint64_t*>(pp)) - 16;
        uint64_t vtable_derived = *(reinterpret_cast<uint64_t*>(pd)) - 16;

        utils::dump("vtable_parent", reinterpret_cast<void*>(vtable_parent), 8 * 5, 8, true, true);
        utils::dump("vtable_derived", reinterpret_cast<void*>(vtable_derived), 8 * 5, 8, true, true);
    }
}