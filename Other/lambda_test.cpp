
#include "lambda_test.hpp"
#include "TypeUtils.hpp"
#include <mutex>
#include <memory>
#include <functional>

namespace lambda_test {

    class C {
    public:
        mutable int i{0};
        C(int i_) :
            i(i_) { std::cout << "C::C(int i)" << std::endl; }
        C() { std::cout << "C::C()" << std::endl; }
        C(C&) { std::cout << "C::C(C&)" << std::endl; }
        C(const C&) { std::cout << "C::C(const C&)" << std::endl; }

        C& operator=(const C&) = delete;
        C& operator=(C&) = delete;
        C(C&&)           = delete;
        C& operator=(C&&) = delete;

        void f() {}
        void fc() const { i += 10; }
    };

    int ext_i{0};

    void test_lambda_local_variables() {
        std::cout << "\ntest_lambda_local_variables" << std::endl;
        int i1{0};
        int i2{1};
        C classc1(1);
        C classc2(2);

        std::cout << "\nauto lambda1 = ..." << std::endl;
        auto lambda1 = [=, &i2, i3 = 1, i4 = int(2), d = 1.1, c = 'a', pi = &i1] {
            // i,d ... константы, даже pi - константная ссылка(не ссылка на константу)
            // = все(те которые используются в теле лямбды) передаваемые объекты копируюся по значению
            // и являются константными, кроме тех кто явно указан для передачи по ссылке

            // можем менять только значения переданные по ссылке
            ++i2;

            // константные объекты менять не разрешается
            // ++i1;
            // classc1.i = 0;

            // но можно вызвать константую функцию
            classc1.fc();

            std::cout << "ext_t type :" << type_utils::type2name<decltype(ext_i)>() << std::endl;
            std::cout << "i1 type    :" << type_utils::type2name<decltype(i1)>() << std::endl;
            std::cout << "i2 type    :" << type_utils::type2name<decltype(i2)>() << std::endl;
            std::cout << "i3 type    :" << type_utils::type2name<decltype(i3)>() << std::endl;
            std::cout << "i4 type    :" << type_utils::type2name<decltype(i4)>() << std::endl;
            std::cout << "d type     :" << type_utils::type2name<decltype(d)>() << std::endl;
            std::cout << "c type     :" << type_utils::type2name<decltype(c)>() << std::endl;
            std::cout << "pi type    :" << type_utils::type2name<decltype(pi)>() << std::endl;
        };

        std::cout << "lambda1() ... " << std::endl;
        lambda1();
        std::cout << "lambda1() ... end" << std::endl;
        // вывод:
        // auto lambda1 = ...
        // C::C(C&)                   копирующий конструктор только для одного класса который используется в теле
        // lambda1() ...
        // i1 type :int               в теле лямбды константная копия с такимже именем, хоть и int но изменять нельзя
        // i2 type :int               в теле лямбды ссылка на внешнюю переменную, можно изменять
        // i3 type :const int
        // i4 type :const int
        // d type  :const double
        // c type  :const char
        // pi type :int* const
        // lambda1() ... end

        std::cout << "\nauto lambda2 = [...] mutable {...}" << std::endl;
        auto lambda2 = [=, &i2, i3 = 1, i4 = int(2), d = 1.1, c = 'a', pi = &i1] mutable {
            // mutable убирает константность с объектов внутри лямбды
            classc1.f();
            classc1.i = 0;

            std::cout << "i1 type :" << type_utils::type2name<decltype(i1)>() << std::endl;
            std::cout << "i2 type :" << type_utils::type2name<decltype(i2)>() << std::endl;
            std::cout << "i3 type :" << type_utils::type2name<decltype(i3)>() << std::endl;
            std::cout << "i4 type :" << type_utils::type2name<decltype(i4)>() << std::endl;
            std::cout << "d type  :" << type_utils::type2name<decltype(d)>() << std::endl;
            std::cout << "c type  :" << type_utils::type2name<decltype(c)>() << std::endl;
            std::cout << "pi type :" << type_utils::type2name<decltype(pi)>() << std::endl;
        };

        std::cout << "lambda2() ... " << std::endl;
        lambda2();
        std::cout << "lambda2() ... end" << std::endl;

        std::cout << "\nauto lambda3 = ..." << std::endl;
        auto lambda3 = [=] {
            classc1.fc();
            std::cout << "classc1.i = " << classc1.i << std::endl;
        };

        std::cout << "lambda3() ... " << std::endl;
        lambda3();
        std::cout << "lambda3() ... " << std::endl;
        lambda3();
        std::cout << "lambda3() ... end" << std::endl;
        std::cout << "classc1.i = " << classc1.i << std::endl;
        // вывод:
        // auto lambda3 = ...
        // C::C(C&)                   копия объекта
        // lambda3() ...
        // classc1.i = 10             константная но переменная внутри объекта mutable
        // lambda3() ...
        // classc1.i = 20             содержимое объекта внутри лямюды сохраняется до следующего вызова
        // lambda3() ... end
        // classc1.i = 1              не изменяет объект вне лямбды


        // note:
        // [&, i] все по ссылке, i по значению
        // [=, &i] все по значению, i по ссылке

        // mutable позволяет изменять копии, но не оригиналы
    }

    void test_lambda_auto_arguments() {
        std::cout << "\ntest_lambda_auto_arguments" << std::endl;

        std::cout << "\nauto lambda1 = ..." << std::endl;
        auto lambda1 = [i = int(0)](auto v) mutable {
            ++i;
            std::cout << "v type :" << type_utils::type2name<decltype(v)>() << std::endl;
            std::cout << "i = " << i << std::endl;
        };

        std::cout << "\nlambda1(int) = ..." << std::endl;
        lambda1(1);
        std::cout << "\nlambda1(int) = ..." << std::endl;
        lambda1(1);
        std::cout << "\nlambda1(int) = ..." << std::endl;
        lambda1(1);
        std::cout << "\nlambda1(char) = ..." << std::endl;
        lambda1('c');
        std::cout << "\nlambda1(int) = ..." << std::endl;
        lambda1(1);

        // note
        // лямбда с auto аргументом аналогична классу с шаблонной функцией
        // (auto&& value) - аналогичен универсальной ссылке, поэтому надо использовать конструкцию:
        // std::forward<decltype(value)>(value)
    }

    void test_lambda() {
        std::cout << "\ntest_lambda" << std::endl;

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