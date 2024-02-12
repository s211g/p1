#include <type_traits>
#include <string>
#include <vector>

#include "template_specialization_test.hpp"


namespace template_specialization_test_ {
    void call_f() {
        std::cout << "\ncall_f() : " << std::endl;
        using namespace template_specialization_test;
        f(1);
        f(double());
        f(std::string());
        f(char());
    }
}

namespace template_specialization_test {

    // ---------------- шаблон класса -------------------

    // "template<>" не требуется
    X<int>::X() {
        std::cout << "X<int>::X()" << std::endl;
    }

    // "template<>" не требуется
    void X<int>::f() {
        std::cout << "X<int>::f()" << std::endl;
    }

    // ---------------- шаблон класса -------------------

    // ---------------- шаблон функции -------------------

    // определение через свободную функцию, будет конфликтовать с явной специализацией "template <> void f<double> {}"
    void f(double t) {
        std::cout << "f(double t)" << std::endl;
    }

    // cпециализация от общего шаблона
    template <>
    void f<std::string>(std::string t) {
        std::cout << "f<std::string>(std::string t)" << std::endl;
    }

    // cпециализация от общего шаблона, Т выводится из аргумента
    // поэтому можно опустить после имени специализацию в скобках: f<>(...) или f<char>(...)
    template <>
    void f(char t) {
        std::cout << "f<>(char t)" << std::endl;
    }

    // ---------------- шаблон функции -------------------

    void test_outside_definitions() {
        std::cout << "\n test_outside_definitions" << std::endl;

        // при полной(явной) специализации "template<>" либо перед явно специализированным классом(в .hpp), либо перед явно специализированной функцией(в .cpp)
        // "template<>" перед реализацией члена класса вне класса не надо

        std::cout << "\n class definitions : " << std::endl;
        // общий шаблон
        X<double> x;
        x.f();
        // вывод:
        // X<T>::X()
        // X<T>::f()

        // явно специализированный шаблон
        X<int> x2;
        x2.f();
        // вывод:
        // X<int>::X()
        // X<int>::f()

        std::cout << "\n function definitions f() : " << std::endl;
        f(1);
        f(double());
        f(std::string());
        f(char());
        // вывод:
        // f<T>(T t) общий шаблон
        // f(double t) переопределено через свободную функцию, если не объявить ее в заголовке то может вызваться шаблонная "ft<T>(T t)
        // f<std::string>(std::string t)  специализация
        // f<>(char t) специализация, с автоматическим выводом Т из аргумента

        // !!! перед возвращаемым типом можно написать inline и реализацию перенести в заголовок

        // "template <>" сообщает что дальнейшее определение класса или функции относится к явной специализации
        // и после имени фукции можно не писать ни <Тип> ни <>

        // !!! в заголовке надо объявить сигнатуры явных специализаций и свободных функций

        template_specialization_test_::call_f(); // если в заголовке не объявлены все специализации и сигнатуры свободных функций вывод может не совпасть
    }

    template <typename T1, typename T2>
    class A {
    public:
        A() {
            std::cout << "A<T1, T2>::A()" << std::endl;
        }
        A(T1 t1, T2 t2) {
            std::cout << "A<T1, T2>::A(t1,t2)" << std::endl;
        }

        int sum(T1 t1, T2 t2) {
            std::cout << "A<T1, T2>::sum(t1,t2)" << std::endl;
            return t1 + t2;
        }
    };

    template <>
    class A<int, char> {
    public:
        A();
        // A() {
        //     std::cout << "A<int, char>::A()" << std::endl;
        // }

        A(int t1, int t2) {
            std::cout << "A<int, char>::A(int,char)" << std::endl;
        }

        template <typename Tx, typename Ty>
        int sum(Tx t1, Ty t2) {
            std::cout << "A<int, char>::sum(t1,t2)" << std::endl;
            return t1 + static_cast<int>(t2);
        }

        void f();
    };

    // определение членов вне класса
    //template<> - не требуется
    A<int, char>::A() {
        std::cout << "A<int, char>::A()" << std::endl;
    }
    void A<int, char>::f() {}

    void test_explicit_specialization() {
        std::cout << "\n test_explicit_specialization" << std::endl;

        A<int, int> a1;
        a1.sum(1, 2);
        //A<int, char> a2;
        //a2.sum(1, 2);

        // выводт конкретизации из аргументов
        A a3(1, 2); // A<T1, T2>::A(t1,t2)
        char c = 1;
        A a4(1, c); // A<int, char>::A(int,char)
        double d;
        A a5(1, d); // A<T1, T2>::A(t1,t2)
    }
}
