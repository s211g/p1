#include <type_traits>
#include <string>
#include <vector>

#include "template_test.hpp"
#include "TStack.hpp"

namespace template_lambda_test {

    template <typename T>
    class A : public T {
    public:
        A() { std::cout << "A::A<T>(){}" << std::endl; }

        A(T& t) { std::cout << "A::A(T&){}" << std::endl; }

        // A(T& t) :
        //     T(t) { std::cout << "A::A(T&):T(t){}" << std::endl; }

        // A(T t) :
        //     T(t) { std::cout << "A::A(T t):T(t){}" << std::endl; }

        T getLambda() { return T(); }
    };

    template <typename T>
    class B : public T {
    public:
        B(T&& t) :
            T(std::forward<T>(t)) { std::cout << "B::B(T&& t): T(std::forward<T>(t)){}" << std::endl; }
        //B(T&& t) { std::cout << "B::B(T&&){}" << std::endl; }
    };

    void test1() {
        std::cout << "\ntest1" << std::endl;

        std::cout << "\nlambda:" << std::endl;
        auto lambda = [](int i) { std::cout << "lambda l(int), i = " << i << std::endl; };
        lambda(1);

        std::cout << "\na1:" << std::endl;
        A<decltype(lambda)> a1;
        a1(2);

        std::cout << "\na2:" << std::endl;
        A a2(lambda);
        a2(3);

        std::cout << "\nalambda:" << std::endl;
        auto alambda = a2.getLambda();
        alambda(4);

        std::cout << "\nb1:" << std::endl;
        // B b(lambda);        //не может вывести тип если только один конструктор с универсальной ссылкой
        B b1(std::move(lambda)); // может вывести тип если только один конструктор с универсальной ссылкой
        b1(5);

        std::cout << "\nb2:" << std::endl;
        // временный объект подходит для семантики универсальной ссылки
        B b2([](int i) { std::cout << "[](int){} i = " << i << std::endl; });
        b2(6);

        // note:
        // в случае лямбды нет необходимости копировать объект, достаточно типа
    }

    template <typename... T>
    class LambdaCollection : T... {
    public:
        LambdaCollection(T&&... t) :
            T(std::forward<T>(t))... {}

        using T::operator()...;
    };

    void test2() {
        std::cout << "\ntest2" << std::endl;

        LambdaCollection lc{
            [](int i) { std::cout << "[](int){} i = " << i << std::endl; },
            [](int* i) { std::cout << "[](int*){} i = " << i << std::endl; },
            [](char c) { std::cout << "[](char){} c = " << c << std::endl; },
        };

        int i;
        lc(1);
        lc(&i);
        lc('a');
        // вывод:
        // [](int){} i = 1
        // [](int*){} i = 0xd4809ff858
        // [](char){} c = a
    }
}
