#include <type_traits>
#include <string>
#include <vector>

#include "template_specialization_test.hpp"

namespace template_specialization_test {

    X<int>::X() {
        std::cout << "X<int>::X()" << std::endl;
    }

    void X<int>::f() {
        std::cout << "X<int>::f()" << std::endl;
    }

    // можно через свободную функцию
    // void f(std::string t) {
    //     std::cout << "f(std::string t)" << std::endl;
    // }

    // или через специализированную от общего шаблона
    template <>
    void f<std::string>(std::string t) {
        std::cout << "f<std::string>(std::string t)" << std::endl;
    }

    void test_outside_definitions() {
        std::cout << "\n test_outside_definitions" << std::endl;

        X<double> x;
        x.f();
        // X<T>::X()
        // X<T>::f()

        X<int> x2;
        x2.f();
        // X<int>::X()
        // X<int>::f()

        f(1);
        f(std::string());
        // f<T>(T t)
        // f(std::string t) или  f<std::string>(std::string t)  как переопределено через свободную или специализированную функцию
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
