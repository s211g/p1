#include <iostream>
#include <functional>
#include <map>
#include <vector>

#include "pointer_to_class_member_test.hpp"
#include "Utils.hpp"

namespace pointer_to_class_member_test {

    class A1 {
    public:
        long long a1{0};
        void fa1(){};
    };

    class A2 {
    public:
        long long a2{0};
        void fa2(){};
    };

    class B : public A1, public A2 {
    public:
        void f1() { std::cout << "B::f1() i = " << i << " j = " << j << std::endl; }
        void f2() { std::cout << "B::f2() i = " << i << " j = " << j << std::endl; }
        int f3(int, double) {
            std::cout << "int B::f3(int, double)" << std::endl;
            return 0;
        }
        typedef int Type1;
        int i{1};
        double d{3};
        int j{1};
        struct S {
            int j{2};
        };
        S s;
    };

    void test1() {
        std::cout << "\ntest1" << std::endl;

        // Объявление указателя на член класса выглядит так:
        // <Тип> <Название класса>::*<Название указателя>;
        // Присвоить указателю смещение:
        // <Название указателя> = &<Название класса>::<Название поля>;

        // доступ к членам:
        // <объект>.*<Название указателя>
        // <указатель на объект>->*<Название указателя>

        B b;
        B* pb      = &b;
        int B::*pi = &B::i;
        int B::*pj = &B::j;

        std::cout << "sizeof(pi) = " << sizeof(pi) << " adr = " << utils::membAdrToVoid(pi) << std::endl;
        std::cout << "sizeof(pj) = " << sizeof(pj) << " adr = " << utils::membAdrToVoid(pj) << std::endl;
        std::cout << "b.*pi = " << b.*pi << std::endl;
        std::cout << "pb->*pi = " << pb->*pi << std::endl;
        // вывод:
        // sizeof(pi) = 8 adr = 0x10
        // sizeof(pj) = 8 adr = 0x20
        // b.*pi = 1
        // pb->*pi = 1

        // Объявление указателя на метод класса выглядит так:
        // <Тип> (<Название класса>::*<Название указателя>)([<Тип1>[, ...,
        //                                                   <ТипN>]]);
        // Присвоить указателю смещение метода внутри класса можно так:
        // <Название указателя> = &<Название класса>::<Название метода>;

        // вызов метода:
        // (<объект>.*<Название указателя>)()
        // (<указатель на объект>->*<Название указателя>)()

        void (B::*pf2)()           = &B::f2;
        int (B::*pf3)(int, double) = &B::f3;
        std::cout << "sizeof(pf2) = " << sizeof(pf2) << " adr = " << utils::membAdrToVoid(pf2) << " offset = " << utils::membAdrToVoid2(pf2) << std::endl;
        std::cout << "sizeof(pf3) = " << sizeof(pf3) << " adr = " << utils::membAdrToVoid(pf3) << " offset = " << utils::membAdrToVoid2(pf3) << std::endl;
        (b.*pf2)();
        // вывод:
        // sizeof(pf2) = 16 adr = 0x7ff7d552ac20 adr2 = 0
        // sizeof(pf3) = 16 adr = 0x7ff7d552aca0 adr2 = 0
        // B::f2() i = 1 j = 1

        void (B::*pfa1)() = &B::fa1;
        void (B::*pfa2)() = &B::fa2;
        std::cout << "sizeof(pfa1) = " << sizeof(pfa1) << " adr = " << utils::membAdrToVoid(pfa1) << " offset = " << utils::membAdrToVoid2(pfa1) << std::endl;
        std::cout << "sizeof(pfa2) = " << sizeof(pfa2) << " adr = " << utils::membAdrToVoid(pfa2) << " offset = " << utils::membAdrToVoid2(pfa2) << std::endl;
        // вывод:
        // sizeof(pfa1) = 16 adr = 0x7ff69e26aed0 adr2 = 0
        // sizeof(pfa2) = 16 adr = 0x7ff69e26aee0 adr2 = 0x8

        //class B : public A1, public A2
        // данные внутри объекта класса B
        // long long A1::a1       - offset = 0
        // long long A2::a2       - offset = 8
        // ... данные касса B     - offset = 16(0x10)

        // в указателе не метод два поля по 8 байт
        // - адрес функции
        // - смещение данных
    }
}