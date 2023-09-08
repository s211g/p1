
#include "rvalue_test.hpp"
#include "constructor_test.hpp"

namespace constructor_test {


    using namespace std;

    class Base {
    public:
        Base() { cout << "Base()" << endl; }
        Base(const Base& other) { cout << "Base(Base&)" << endl; }
        explicit Base(int i) :
            num(i) { cout << "Base(int)" << endl; }
        explicit Base(char c) :
            letter(c) { cout << "Base(char)" << endl; }

    private:
        int num;
        char letter;
    };

    class Derived : Base {
    public:
        // Inherit all constructors from Base
        using Base::Base;

    public:
        // Can't initialize newMember from Base constructors.
        int newMember{123};
    };

    void test1() {
        std::cout << "test1" << std::endl;

        cout << "Derived d1(5) calls: ";
        Derived d1(5);
        cout << "Derived d1('c') calls: ";
        Derived d2('c');
        cout << "Derived d3 = d2 calls: ";
        Derived d3 = d2;
        cout << "Derived d4 calls: ";
        Derived d4;

        std::cout << "d4.newMember = " << d4.newMember << std::endl;

        // Шаблон класса может наследовать все конструкторы от аргумента типа, если этот тип определяет базовый класс :
        //     template <typename T>
        //     class Derived : T {
        //     using T::T; // declare the constructors from T
        //     // ...
        // };
        // Производный класс не может наследовать от нескольких базовых классов, если эти базовые классы имеют
        // конструкторы с идентичной сигнатурой.

        // Классы, содержащие члены типа класса, называются составными классами.
        // При создании члена типа класса составного класса конструктор вызывается перед собственным конструктором класса.
        // class Label {
        // public:
        //     Label(int){};
        // };
        // class StorageBox {
        // public:
        //     StorageBox() :
        //         m_label(0) {}
        // private:
        //     Label m_label;
        // };
    }
}