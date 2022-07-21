#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <functional>
#include <memory>

#include "smartpointer_test.hpp"

namespace smartpointer_test {

    class A {
    public:
        A(int i_) :
            i(i_) {}
        ~A() {
            std::cout << "~A(" << i << ")" << std::endl;
        }
        void f() { std::cout << "f = " << i << std::endl; }

    private:
        int i;
    };

    void test_shared_ptr() {
        std::cout << "test std::shared_ptr" << std::endl;

        A* a = new A(-1);
        std::shared_ptr<A> ptr_a;
        ptr_a.reset(a);
        ptr_a->f();
        std::shared_ptr<A> ptr_a0(new A(0));
        ptr_a0->f();


        ptr_a0.swap(ptr_a);
        ptr_a->f();
        ptr_a0->f();


        std::shared_ptr<A> ptr_a1 = std::make_shared<A>(1);
        ptr_a1->f();
        std::shared_ptr ptr_a2 = std::make_shared<A>(2);
        ptr_a2->f();
        auto ptr_a3 = std::make_shared<A>(3);
        ptr_a3->f();

        std::shared_ptr<A> ptr_a4;
        ptr_a4 = ptr_a3;
        std::cout << "ptr_a4 = " << (ptr_a4 ? ptr_a4.get() : nullptr) << std::endl;
        ptr_a4->f();

        std::shared_ptr<A> ptr_a5;
        ptr_a5 = std::move(ptr_a4);
        ptr_a5->f();
        std::cout << "ptr_a4 = " << (ptr_a4 ? ptr_a4.get() : nullptr) << std::endl;
    }

    void test_unique_ptr() {
        std::cout << "test std::unique_ptr" << std::endl;

        auto u0 = std::make_unique<A>(0);
        u0->f();

        std::unique_ptr<A> u1;
        u1 = std::move(u0);
        u1->f();

        std::unique_ptr<A> u2;
        u2.reset(u1.release());
        u2->f();

        std::unique_ptr<A> u3;
        u3.swap(u1);
        u3->f();
    }

    void test_weak_ptr() {
        std::cout << "test std::weak_ptr" << std::endl;
        auto s0 = std::make_shared<A>(0);
        s0->f();
        {
            std::weak_ptr w0(s0);
            auto s1 = w0.lock();
            s0.reset();
            s1->f();
            auto s2 = w0.lock();
            std::cout << "s2 = " << (s2 ? s2.get() : nullptr) << std::endl;

            s1.reset();
            s2.reset();
            auto s3 = w0.lock();
            std::cout << "s3 = " << (s3 ? s3.get() : nullptr) << std::endl;
        }
    }
}
