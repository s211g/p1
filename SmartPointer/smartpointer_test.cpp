#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <functional>
#include <memory>
#include <cassert>

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
        int i;
    };

    void test_shared_ptr() {
        std::cout << "\ntest std::shared_ptr" << std::endl;

        std::cout << "\ntest 1" << std::endl;
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

        std::cout << "\ntest 6" << std::endl;
        struct AA1 {
            int i;
            int j;
            AA1(int i_, int j_) :
                i(i_), j(j_) {}
            ~AA1() { std::cout << "AA1::~AA1() i = " << i << ", j = " << j << std::endl; }
        };
        struct AA2 {
            int i;
            AA2(int i_) :
                i(i_) {}
            ~AA2() { std::cout << "AA2::~AA2() i = " << i << std::endl; }
        };

        // совместное использование управляющего блока(при удалении удаляется лиш тот кусок памяти на который указывает управляющий блок)
        // указатель на память в самом умном птр может указывать вобще в другое место
        {
            std::shared_ptr<AA2> p_aa2;
            {
                auto ptr_aa1 = std::make_shared<AA1>(1, 2);
                AA2* aa2     = new AA2(3);
                p_aa2        = std::shared_ptr<AA2>(ptr_aa1, aa2);
            }
            std::cout << " p_aa2->i = " << p_aa2->i << std::endl;
            // здесь разрушается управляющий блок AA1 и  вызывается деструктор AA1
            // а вот деструктор AA2 никогда не вызовется
        }
        std::cout << "------" << std::endl;
    }

    void test_unique_ptr() {
        std::cout << "\ntest std::unique_ptr" << std::endl;

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
        std::cout << "\ntest std::weak_ptr" << std::endl;
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


    class StorageA {
    public:
        StorageA() :
            m_counter(0){};
        ~StorageA() = default;

        std::unique_ptr<A, StorageA&> New() {
            if (m_va.empty()) {
                std::cout << "A new a.i = " << m_counter << std::endl;
                return std::unique_ptr<A, StorageA&>(new A(m_counter++), *this);
            }

            A* a = m_va.back();
            m_va.pop_back();
            std::cout << "A new from vector a.i = " << a->i << " m_va.size = " << m_va.size() << std::endl;
            return std::unique_ptr<A, StorageA&>(a, *this);
        }

        void operator()(A* a) {
            m_va.push_back(a);
            std::cout << "A delete a.i = " << a->i << " m_va.size = " << m_va.size() << std::endl;
        }

    private:
        uint32_t m_counter;
        std::vector<A*> m_va;
    };

    void test_deleter() {
        std::cout << "\ntest deleter" << std::endl;
        StorageA sa;

        auto ua0 = sa.New();
        auto ua1 = sa.New();
        auto ua2 = sa.New();

        ua0.reset();
        ua1.reset();

        auto ua3 = sa.New();
        auto ua4 = sa.New();
        auto ua5 = sa.New();

        std::cout << "----------------" << std::endl;
    }

    class Aesft : public std::enable_shared_from_this<Aesft> {
    public:
        Aesft() { std::cout << "Aesft()" << std::endl; }
        Aesft(const Aesft&) { std::cout << "Aesft(const Aesft&)" << std::endl; }
        ~Aesft() { std::cout << "~Aesft()" << std::endl; }

        template <class F>
        void f1(const F& fn) {
            std::cout << "A::f()" << std::endl;
            fn(this->shared_from_this());
        }
        void f2() { std::cout << "A::f2()" << std::endl; }
    };

    void test_enable_shared_from_this() {
        std::cout << "\ntest std::enable_shared_from_this" << std::endl;

        auto cb = [](auto sa) {
            std::cout << "use count: " << sa.use_count() << std::endl;
            sa->f2(); };

        std::cout << "\ntest 1" << std::endl;
        {
            auto saefs = std::make_shared<Aesft>();
            assert(saefs.use_count() == 1);
            saefs->f1(cb);
        }

        std::cout << "\ntest 2" << std::endl;
        {
            Aesft aefst;
            try {
                aefst.f1(cb);
            }
            catch (const std::bad_weak_ptr& ex) {
                std::cout << "exception : " << ex.what() << std::endl;
            }
        }
    }
}