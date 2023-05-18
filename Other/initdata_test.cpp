
#include "initdata_test.hpp"
#include "TypeUtils.hpp"
#include <mutex>
#include <memory>
#include <functional>

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

    void test_lambda() {
        std::cout << "test lambda" << std::endl;

        // Общее:

        // в замыкание все значения передаются как const копии
        // чтобы переместить не копируемые объекты(напр unique_ptr) надо сделать их инициализацию в лямбда выражении:
        // [var = move(var)]

        // !!! при объявлении ламбда выражений в теле функций класса с видимостью [=]
        // члены класса становятся доступны неявно через скрытый this,
        // время сущестования ляьбды в этом случае не должно превышать
        // времени существования объекта этого класса

        // для вывода типа аргументов [](auto&&... arg) в теле лямбды надо использовать decltype():
        // f(std::forward<decltype(arg)>(arg)...);

        std::cout << "\ntest 1 move objects to lambda" << std::endl;
        class A {
        public:
            ~A() {
                std::cout << "A::~A(int) reset i to -1" << std::endl;
                i = -1;
            }
            A(int i_) :
                i(i_) { std::cout << "A::A(int) i = " << i << std::endl; }
            A(const A& a) :
                i(a.i) { std::cout << "A::A(const A&) i = " << i << std::endl; }
            int i{0};
            A(A&& a) {
                i   = a.i;
                a.i = 0;
                std::cout << "A::A(A&&) i = " << i << std::endl;
            }

            auto ret_lambda() {
                return [=] { std::cout << "i = " << i << std::endl; };
            }
        };

        {
            A a1(1);
            std::unique_ptr<A> ua1 = std::make_unique<A>(2);
            int j                  = 123;
            auto l                 = [=, ua = std::move(ua1), pa = new A(3)] {
                std::cout << "j = " << j << std::endl;
                std::cout << "a1.i = " << a1.i << std::endl;   // объект a1 - копия a1 до объявления ламбды
                std::cout << "ua->i = " << ua->i << std::endl; // ua - перенесено через инициализирующее объявление
                std::cout << "pa->i = " << pa->i << std::endl; // создано при объявлении
            };

            l();
        }
        std::cout << "\ntest 2 lambda and class members" << std::endl;

        std::function<void()> f2;
        {
            A a2{123};
            auto l2 = a2.ret_lambda();
            l2();
            a2.i = 456;
            l2();
            f2 = l2;
            f2();
        }
        f2(); // висячая ссылка на член класса внутри лямбды, лямбда ссылается на удаленный объект

        std::cout << "\ntest 3 lambda(auto&&)" << std::endl;
        auto l3 = [](auto&&... arg) {
            std::cout << "arg type : " << ((type_utils::type2name<decltype(arg)>() + " ") + ...) << std::endl;
            {
                A a(std::forward<decltype(arg)>(arg)...);
            }
            std::cout << std::endl;
        };
        l3(1); // arg type : int&&, A::A(int) i = 1
        A a3(3);
        l3(a3);   // arg type: A&, A::A(const A&) i = 3
        l3(A{4}); // arg type : A&&, A::A(A&&) i = 4
    }

}