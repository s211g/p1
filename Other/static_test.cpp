
#include "rvalue_test.hpp"
#include "static_test.hpp"

namespace static_test {

    template <>
    int C<int>::i{1};


    // Static - это ключевое слово в C++, используемое для придания элементу особых характеристик.
    // Для статических элементов выделение памяти происходит только один раз и существуют эти
    // элементы до завершения программы. Хранятся все эти элементы не в heap и не на stack,
    // а в специальных сегментах памяти, которые называются .data и .bss (зависит от того
    // инициализированы статические данные или нет). На картинке ниже показан типичный макет
    // программной памяти.

    A B::a;
    //A B::a(123);

    void f1(int i) { static A a(i); };

    void test1() {
        std::cout << "test1" << std::endl;

        std::cout << "B::a.i =" << B::a.i << std::endl;

        // если переменная внутри тела функции то инициализация один раз при первом входе в область видимости
        auto f2 = [](int i) { static A a(i); };
        std::cout << "f1() ... --------------------" << std::endl;
        f1(1);
        f1(2);
        std::cout << "f2() ... --------------------" << std::endl;
        f2(3);
        f2(4);
        std::cout << "--------------------" << std::endl;
        B b;
        std::cout << "b.a.i = " << b.a.i << std::endl;
    }

    void test2() {
        std::cout << "test2" << std::endl;

        // статическая функция имеет доступ к приват и протектед секции
        B b;
        B::printJK(b);
    }

    void test_static_in_template() {
        std::cout << "test_static_in_template" << std::endl;

        C<float>::i += 1;

        C<float> c_float;
        std::cout << "c_float.i = " << c_float.i << std::endl;
        C<char> c_char;
        std::cout << "c_char.i = " << c_char.i << std::endl;
        C<int> c_int;
        std::cout << "c_int.i = " << c_int.i << std::endl;
        // вывод:
        // c_float.i = -9
        // c_char.i = -10
        // c_int.i = 1
    }


}