
#include "functional_hdr_test.hpp"
#include <functional>

namespace functional_hdr_test {

    class A_bind {
    public:
        void f(int i, int j, int k) { std::cout << "bind f(" << i << ", " << j << ", " << k << ")" << std::endl; }
    };

    void f_bind(int i, int j, int k) { std::cout << "bind f(" << i << ", " << j << ", " << k << ")" << std::endl; }

    void test_bind() {
        std::cout << "test std::bind " << std::endl;

        using std::placeholders::_1;
        using std::placeholders::_2;
        std::function<void(int, int)> bf = std::bind(f_bind, 0, _2, _1);
        bf(1, 2);

        A_bind a;
        std::function<void(int, int)> bf1 = std::bind(&A_bind::f, a, 0, _2, _1);
        bf1(1, 2);
    }

    void test_function() {
        std::cout << "test std::function " << std::endl;
    }

}
