
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

    void f_function(int i) { std::cout << "f_function(" << i << ")" << std::endl; }

    class A_function {
    public:
        A_function(int i_) :
            i(i_) {}
        int i;

        void add(int j) const { std::cout << "A::add() i = " << i << " j = " << j << " result = " << i + j << std::endl; }
        void replace(int j) {
            std::cout << "A::replace() i = " << i << " j = " << j << " ";
            i = j;
            std::cout << " result i = " << i << std::endl;
        }

        void operator()(int j) const { std::cout << "operator()(int j) " << j << std::endl; }
        void operator()() const { std::cout << "operator()() " << std::endl; }
    };

    void test_function() {
        std::cout << "test std::function " << std::endl;

        std::function<void(int)> f1 = f_function;
        f1(1);

        std::function<void(int)> f2 = [](int i) { std::cout << "lambda i = " << i << std::endl; };
        f2(2);

        // skip f = bind, see above

        std::function<void(const A_function&, int)> f3 = &A_function::add;
        A_function af(1);
        f3(af, 2);
        f3(3, 2);
        f3(A_function(0), 2);

        std::function<void(A_function&, int)> f4 = &A_function::replace;
        f4(af, 9);

        // store a call to a data member accessor
        // const
        std::function<int(A_function const&)> f5 = &A_function::i;
        std::cout << "A_function::i = " << f5(af) << std::endl;
        // non const
        std::function<int&(A_function&)> f6 = &A_function::i;
        f6(af)                              = 10;
        std::cout << "f6(af) = 10; af.i = " << af.i << std::endl;

        // store a call to a function object
        std::function<void()> f7 = af;
        f7();
        std::function<void(int)> f8 = af;
        f8(1);

        // recursive
        std::function<int(int)> factorial = [&](int n) { return (n < 2) ? 1 : n * factorial(n - 1); };
        std::cout << "factorial(3)= " << factorial(3) << std::endl;
        // old variant
        std::function<int(int)> factorial_old;
        factorial_old = [&factorial_old](int n) { return (n < 2) ? 1 : n * factorial_old(n - 1); };
        std::cout << "factorial_old(3)= " << factorial_old(3) << std::endl;
    }

}
