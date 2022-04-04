#include <iostream>
#include <memory>
#include <functional>

template <typename T>
class X {
public:
    void f() {
        static_cast<T*>(this)->f();
    }
};

class A : public X<A> {
public:
    void f() {
        std::cout << "hiiii " << std::endl;
    }
};

class Y {
public:
    int i;

    Y() :
        i(0) {}

    Y(int i_) :
        i(i_) {}

    void f(int j) const {
        std::cout << "f : " << i << " j: " << j << std::endl;
    }

    void ff() const {
        std::cout << "ff : " << i << std::endl;
    }

    void operator()() {
        std::cout << "0" << std::endl;
    }

    void operator()(int) {
        std::cout << "1" << std::endl;
    }
};

struct Foo {
    Foo(int num) :
        num_(num) {}
    void print_add(int i) const { std::cout << num_ + i << '\n'; }
    int num_;
};

void print_num(int i) {
    std::cout << i << '\n';
}

struct PrintNum {
    void operator()(int i) const {
        std::cout << i << '\n';
    }
};

int main() {
    auto pf = &Y::f;
    Y y;
    (y.*pf)(888);

    std::function<void()> fn = y;
    fn();
    std::function<void(int)> fn1 = y;
    fn1(1);

    auto py = std::make_unique<Y>(777);
    //    std::function<void(std::unique_ptr<Y>)> fn100 = &Y::ff;
    std::function<void(Y*)> fn100 = &Y::ff;
    fn100(py.get());
    std::function<void(Y&)> fn101 = &Y::ff;
    fn101(*py);

    std::function<void(Y*)> fn10 = &Y::ff;
    fn10(&y);

    std::function<void(const Y&)> fn11 = &Y::ff;
    fn11(y);
    fn11(123555);
    auto memfn11 = std::mem_fn(&Y::ff);
    memfn11(y);
    memfn11(Y(5555));

    std::function<void(const Y&, int)> fn2 = &Y::f;
    fn2(y, 1);
    fn2(Y(223), 1);

    std::function<int(Y const&)> fn3 = &Y::i;
    std::cout << "Y::i " << fn3(987) << std::endl;

    //std::function<void(int)> fn4 = std::bind(&Y::f, y, std::placeholders::_1);
    std::function<void(const Y&)> fn4 = std::bind(&Y::f, std::placeholders::_1, 123);
    fn4(y);
    fn4(555);

    std::function<void(Y*)> fn5 = std::bind(&Y::f, std::placeholders::_1, 124);
    fn5(&y);

    auto fn6 = std::bind(&Y::f, std::placeholders::_1, 12499);
    fn6(&y);


    return 0;

    // store a free function
    std::function<void(int)> f_display = print_num;
    f_display(-9);

    // store a lambda
    std::function<void()> f_display_42 = []() { print_num(42); };
    f_display_42();

    // store the result of a call to std::bind
    std::function<void()> f_display_31337 = std::bind(print_num, 31337);
    f_display_31337();

    // store a call to a member function
    std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(314159);
    f_add_display(foo, 1);
    f_add_display(314159, 1);

    // store a call to a data member accessor
    std::function<int(Foo const&)> f_num = &Foo::num_;
    std::cout << "num_: " << f_num(foo) << '\n';

    // store a call to a member function and object
    using std::placeholders::_1;
    std::function<void(int)> f_add_display2 = std::bind(&Foo::print_add, foo, _1);
    f_add_display2(2);

    // store a call to a member function and object ptr
    std::function<void(int)> f_add_display3 = std::bind(&Foo::print_add, &foo, _1);
    f_add_display3(3);

    // store a call to a function object
    std::function<void(int)> f_display_obj = PrintNum();
    f_display_obj(18);

    auto factorial = [](int n) {
        // store a lambda object to emulate "recursive lambda"; aware of extra overhead
        std::function<int(int)> fac = [&](int n) { return (n < 2) ? 1 : n * fac(n - 1); };
        // note that "auto fac = [&](int n){...};" does not work in recursive calls
        return fac(n);
    };
    for (int i{5}; i != 8; ++i) { std::cout << i << "! = " << factorial(i) << ";  "; }

    return 0;
}
