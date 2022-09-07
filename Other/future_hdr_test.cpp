#include "functional_hdr_test.hpp"
#include <future>
#include <string>

namespace future_hdr_test {

    class A {
    public:
        int i{0};
        A() { std::cout << "A::A()" << std::endl; }
        A(A& a) {
            std::cout << "A::A(A&)" << std::endl;
            i = a.i;
        }
        A(const A& a) {
            std::cout << "A::A(const A&)" << std::endl;
            i = a.i;
        }
        A(A&& a) {
            std::cout << "A::A(A&&)" << std::endl;
            i = a.i;
        }

        A(int i_) :
            i(i_) { std::cout << "A::A(int)" << std::endl; }
        void operator()() const { std::cout << "A::operator()() i = " << i << std::endl; }
        void operator()(int i_) const { std::cout << "A::operator()(int i_ ) i_ = " << i_ << ", i = " << i << std::endl; }
        void fn() { std::cout << "A::fn()" << std::endl; }
        void fn(int i_) {
            std::cout << "A::fn(int) i = " << i_ << std::endl;
            i = i_;
        }
        void print() { std::cout << "A::print() i = " << i << std::endl; }
    };

    struct B {
        int i{0};
        void print() { std::cout << "B::print() i = " << i << std::endl; }
    };

    B fn(B&);
    B fn(B& b) {
        std::cout << "call B fn(B& b) b.i = " << b.i << std::endl;
        return b;
    }

    void test_async() {
        std::cout << "test std::async" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        B b{11};
        std::future<B> f1 = std::async(fn, std::ref(b));
        B b1              = f1.get(); // call fn(B& b)
        b1.print();

        //        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        std::cout << "\ntest 2" << std::endl;
        A a2(2);
        //auto f2 = std::async(a2); // not correct, need ref; call A(a2).operator()
        auto f2 = std::async(std::ref(a2));
        f2.wait(); // call a2.operator()

        std::cout << "\ntest 3" << std::endl;
        auto f3 = std::async(A(3));
        f3.wait(); // call temp_A(3)()

        std::cout << "\ntest 4" << std::endl;
        A a4(4);
        //auto f4 = std::async(&A::print, a4); f4.wait(); // call A(a4).print()
        auto f4 = std::async(&A::print, std::ref(a4));
        f4.wait(); // call a4.print()

        // std::launch::async - new thread
        // std::launch::deferred - same thread
        std::cout << "\ntest 5" << std::endl;
        auto f5 = std::async(std::launch::deferred, [] { std::cout << "Hi from thread" << std::endl; });
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "std::async(deferred, ...) ... " << std::endl;
        f5.wait();

        std::cout << "\ntest 6" << std::endl;
        auto f6 = std::async([] {
            std::cout << "Start thread" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "End thread" << std::endl;
        });
        if (std::future_status::timeout == f6.wait_for(std::chrono::seconds(1))) {
            std::cout << "Wait timeout" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}