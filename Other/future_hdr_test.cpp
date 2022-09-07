#include "functional_hdr_test.hpp"
#include <future>
#include <string>

namespace future_hdr_test {

    class A {
    public:
        A() = default;
        void operator()() const { std::cout << "A::operator()()" << std::endl; }
    };

    void test_async() {
        std::cout << "test std::async" << std::endl;

        A a();
        a();
    }
}