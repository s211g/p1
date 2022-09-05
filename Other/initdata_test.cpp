
#include "initdata_test.hpp"
#include <mutex>

namespace initdata_test {

    void call_once1(int i) {
        std::cout << "call_once test 1, i = " << i << std::endl;
    }

    std::once_flag once_flag1;
    std::once_flag once_flag2;

    class A {
    public:
        std::once_flag once_flag3;
        int i{1};
        void test() { std::cout << "call_once test 3, i = " << i << std::endl; }
        void call_once() {
            std::call_once(once_flag3, &A::test, this);
            ++i;
        }
    };

    void test_call_once() {
        std::cout << "test std::call_once" << std::endl;

        std::call_once(once_flag1, call_once1, 1);
        std::call_once(once_flag1, call_once1, 2);

        std::call_once(once_flag2, [] {
            std::cout << "call_once test 2" << std::endl;
        });
        std::call_once(once_flag2, [] {
            std::cout << "call_once test 2" << std::endl;
        });

        A a;
        a.call_once();
        a.call_once();
    }
}