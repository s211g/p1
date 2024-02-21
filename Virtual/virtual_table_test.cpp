#include "virtual_function_test.hpp"
#include "Utils.hpp"

namespace virtual_table_test {

    class Parent {
    public:
        long long i{1};
        virtual void Foo() {}
        virtual void FooNotOverridden() {}
    };

    class Derived1 : public Parent {
    public:
        long long j{2};

        virtual void f1() {}
        virtual void Foo() override {}
        virtual void f2() {
            std::cout << "Derived2::f()" << std::endl;
        }
    };

    class A1 {
    public:
        long long a{3};
    };

    class A2 {
    public:
        virtual void f3() {}
        long long a{4};
    };


    class Derived2 : public A1, public A2, public Derived1 {
    public:
        virtual void f2() override {
            std::cout << "Derived2::f()" << std::endl;
        }
    };

    void test1() {
        std::cout << "\ntest1" << std::endl;

        Parent* p               = new Parent();
        Derived1* d1            = new Derived1();
        Derived2* d2            = new Derived2();
        Parent* parent_derived2 = d2;

        utils::dump("Parent", p, sizeof(Parent), 8, true);
        utils::dump("Derived1", d1, sizeof(Derived1), 8, true);
        utils::dump("Derived2", d2, sizeof(Derived2), 8, true);

        uint64_t vtable_parent   = *(reinterpret_cast<uint64_t*>(p)) - 16;
        uint64_t vtable_derived1 = *(reinterpret_cast<uint64_t*>(d1)) - 16;
        uint64_t vtable_derived2 = *(reinterpret_cast<uint64_t*>(d2)) - 16;

        utils::dump("vtable_parent", reinterpret_cast<void*>(vtable_parent), 8 * 8, 8, true, true);
        utils::dump("vtable_derived1", reinterpret_cast<void*>(vtable_derived1), 8 * 8, 8, true, true);
        utils::dump("vtable_derived2", reinterpret_cast<void*>(vtable_derived2), 8 * 12, 8, true, true);

        utils::dump("parent_derived2", parent_derived2, sizeof(Parent), 8, true);
        uint64_t vtable_parent_derived2 = *(reinterpret_cast<uint64_t*>(parent_derived2)) - 16;
        utils::dump("vtable_parent_derived2", reinterpret_cast<void*>(vtable_parent_derived2), 8 * 8, 8, true, true);
        // вывод

        // Parent(0x196ae4c6c10) size = 16 :
        // 0x00000196AE4C6C10  : E0 6A 43 F8 F6 7F 00 00
        // 0x00000196AE4C6C18  : 01 00 00 00 00 00 00 00
        // Derived1(0x196ae4c6c30) size = 24 :
        // 0x00000196AE4C6C30  : 00 6B 43 F8 F6 7F 00 00
        // 0x00000196AE4C6C38  : 01 00 00 00 00 00 00 00
        // 0x00000196AE4C6C40  : 02 00 00 00 00 00 00 00
        // Derived2(0x196ae4c6f80) size = 48 :
        // 0x00000196AE4C6F80  : 30 6B 43 F8 F6 7F 00 00 vptr
        // 0x00000196AE4C6F88  : 04 00 00 00 00 00 00 00 A2::a
        // 0x00000196AE4C6F90  : 03 00 00 00 00 00 00 00 A1::a
        // 0x00000196AE4C6F98  : 50 6B 43 F8 F6 7F 00 00 Parent::i (+24 байта от начала)
        // 0x00000196AE4C6FA0  : 01 00 00 00 00 00 00 00 Derived::j
        // 0x00000196AE4C6FA8  : 02 00 00 00 00 00 00 00

        // vtable_parent(0x7ff6f8436ad0) size = 64 :
        // 0x00007FF6F8436AD0 + 0        : 00 00 00 00 00 00 00 00
        // 0x00007FF6F8436AD8 + 8        : 10 1D 43 F8 F6 7F 00 00
        // 0x00007FF6F8436AE0 + 16       : 50 68 3A F8 F6 7F 00 00 Foo
        // 0x00007FF6F8436AE8 + 24       : 40 68 3A F8 F6 7F 00 00 FooNotOverridden
        // 0x00007FF6F8436AF0 + 32       : 00 00 00 00 00 00 00 00
        // 0x00007FF6F8436AF8 + 40       : 20 1D 43 F8 F6 7F 00 00
        // 0x00007FF6F8436B00 + 48       : E0 68 3A F8 F6 7F 00 00
        // 0x00007FF6F8436B08 + 56       : 40 68 3A F8 F6 7F 00 00


        // vtable_derived1(0x7ff6f8436af0) size = 64 :
        // 0x00007FF6F8436AF0 + 0        : 00 00 00 00 00 00 00 00
        // 0x00007FF6F8436AF8 + 8        : 20 1D 43 F8 F6 7F 00 00
        // 0x00007FF6F8436B00 + 16       : E0 68 3A F8 F6 7F 00 00 Foo переопределили, в таблица адрес функции другой
        // 0x00007FF6F8436B08 + 24       : 40 68 3A F8 F6 7F 00 00 FooNotOverridden, не переопределяли, в таблице адрес такойже
        // 0x00007FF6F8436B10 + 32       : C0 68 3A F8 F6 7F 00 00 f1 - новая виртуальная функция
        // 0x00007FF6F8436B18 + 40       : D0 68 3A F8 F6 7F 00 00 f2 - новая виртуальная функция
        // 0x00007FF6F8436B20 + 48       : 00 00 00 00 00 00 00 00
        // 0x00007FF6F8436B28 + 56       : 40 1D 43 F8 F6 7F 00 00


        // vtable_derived2(0x7ff6f8436b20) size = 96 :
        // 0x00007FF6F8436B20 + 0        : 00 00 00 00 00 00 00 00 vtable offset(derived2, A2) -----------------------------------
        // 0x00007FF6F8436B28 + 8        : 40 1D 43 F8 F6 7F 00 00 vtable info(derived2, A2)
        // 0x00007FF6F8436B30 + 16       : C0 67 3A F8 F6 7F 00 00 A2::f
        // 0x00007FF6F8436B38 + 24       : 70 69 3A F8 F6 7F 00 00 f2 - virtual и переопределена в базовых(если не будет объявлена как virtual то этой строчки не будет, но переопределение в базовых будет)
        // 0x00007FF6F8436B40 + 32       : E8 FF FF FF FF FF FF FF vtable offset(derived1) ---------------------------------------
        // 0x00007FF6F8436B48 + 40       : 40 1D 43 F8 F6 7F 00 00 vtable typeinfo(derived1)
        // 0x00007FF6F8436B50 + 48       : E0 68 3A F8 F6 7F 00 00 Foo, тотже что и у derived1(просто класс не встроенный в иерархию)
        // 0x00007FF6F8436B58 + 56       : 40 68 3A F8 F6 7F 00 00 FooNotOverridden, тот же что и у derived1
        // 0x00007FF6F8436B60 + 64       : C0 68 3A F8 F6 7F 00 00 f1, тот же что и у derived1
        // 0x00007FF6F8436B68 + 72       : 70 B2 41 F8 F6 7F 00 00 f2, переопределили, теперь является фунцией переходником на diraved2::f2, смещает this на начало объекта derived2
        // 0x00007FF6F8436B70 + 80       : 00 00 00 00 00 00 00 00
        // 0x00007FF6F8436B78 + 88       : A0 1D 43 F8 F6 7F 00 00

        // parent_derived2(0x196ae4c6f98) size = 16 :
        // 0x00000196AE4C6F98  : 50 6B 43 F8 F6 7F 00 00
        // 0x00000196AE4C6FA0  : 01 00 00 00 00 00 00 00

        // vtable_parent_derived2(0x7ff6f8436b40) size = 64 :
        // 0x00007FF6F8436B40 + 0        : E8 FF FF FF FF FF FF FF -24 смещение на начала родительского объекта(может не являца началом изначально созданного объекта)
        // 0x00007FF6F8436B48 + 8        : 40 1D 43 F8 F6 7F 00 00
        // 0x00007FF6F8436B50 + 16       : E0 68 3A F8 F6 7F 00 00
        // 0x00007FF6F8436B58 + 24       : 40 68 3A F8 F6 7F 00 00
        // 0x00007FF6F8436B60 + 32       : C0 68 3A F8 F6 7F 00 00
        // 0x00007FF6F8436B68 + 40       : 70 B2 41 F8 F6 7F 00 00
        // 0x00007FF6F8436B70 + 48       : 00 00 00 00 00 00 00 00
        // 0x00007FF6F8436B78 + 56       : A0 1D 43 F8 F6 7F 00 00

        // note:
        // !!!
        // class Derived2 : public A1, public A2, public Derived1 {
        // данные в объекте могут быть расположены не в такомже порядке как в списке наследования,
        // текущий компилятор (gcc (Rev10, Built by MSYS2 project) 11.2.0)
        // данные классов без виртуальных функций, располагает после данных классов с виртуальными
        // какбы оптимизирует порядок наследования, ну естественно меняется порядок вызова конструкторов

        // vtable
        // вначало идут виртуальные!!!(не виртуальных нет) переопределенные методы первого базового(полиморфного) класса
        // далее следуют новые виртуальные методы объявленные в классе
        // дальше идет vtable следующего полиморфного класса в порядке наследования слева направо
    }


    class DELIMITER1 {
    public:
        long long i{1};
        virtual void fx() {}
    };

    class DELIMITER2 {
    public:
        long long i{2};
        virtual void fy() {}
    };

    class B1 {
    public:
        long long j{3};
        virtual void f() {
            std::cout << "B1::f() this[" << this << "] j = " << j << std::endl;
        }
    };

    class B2 : public DELIMITER1, public B1 {
    public:
        long long k{4};
        virtual void f() override {
            std::cout << "B2::f()" << std::endl;
        }
    };

    class B3 : public DELIMITER2, public B2 {
    public:
        long long l{5};
        virtual void f() override {
            std::cout << "B3::f() this[" << this << "] l = " << l << std::endl;
        }
    };

    void test2() {
        std::cout << "\ntest2" << std::endl;

        B3* b3 = new B3();
        utils::dump("B3", b3, sizeof(B3), 8, true);

        // B3(0x26b50736c10) size = 64 :
        // 0x0000026B50736C10  : 60 7D 66 BF F6 7F 00 00
        // 0x0000026B50736C18  : 02 00 00 00 00 00 00 00
        // 0x0000026B50736C20  : 80 7D 66 BF F6 7F 00 00
        // 0x0000026B50736C28  : 01 00 00 00 00 00 00 00
        // 0x0000026B50736C30  : A0 7D 66 BF F6 7F 00 00
        // 0x0000026B50736C38  : 03 00 00 00 00 00 00 00
        // 0x0000026B50736C40  : 04 00 00 00 00 00 00 00
        // 0x0000026B50736C48  : 05 00 00 00 00 00 00 00

        // B3(0x1951cae6c10) size = 64 :
        // 0x000001951CAE6C10 + 0  : 60 7D 0E 39 F6 7F 00 00 vtbl(B3,DELIMITER2)----------------
        // 0x000001951CAE6C18 + 8  : 02 00 00 00 00 00 00 00 DELIMITER2::i                     |
        // 0x000001951CAE6C20 + 16 : 80 7D 0E 39 F6 7F 00 00 vtbl(B2,DELIMITER1) -------       |
        // 0x000001951CAE6C28 + 24 : 01 00 00 00 00 00 00 00 DELIMITER1::i             |       |
        // 0x000001951CAE6C30 + 32 : A0 7D 0E 39 F6 7F 00 00 vtbl(B1) ---------B1      B2      B3
        // 0x000001951CAE6C38 + 40 : 03 00 00 00 00 00 00 00 B1::j -------------       |       |
        // 0x000001951CAE6C40 + 48 : 04 00 00 00 00 00 00 00 B2::k----------------------       |
        // 0x000001951CAE6C48 + 56 : 05 00 00 00 00 00 00 00 B3::l------------------------------
        // vtable_B3(0x7ff6390e7d50) size = 160 :
        // 0x00007FF6390E7D50 + 0        : 00 00 00 00 00 00 00 00 vtbl(B3,DELIMITER2)
        // 0x00007FF6390E7D58 + 8        : C0 2D 0E 39 F6 7F 00 00 type_info
        // 0x00007FF6390E7D60 + 16       : F0 69 05 39 F6 7F 00 00 DELIMITER2::fy
        // 0x00007FF6390E7D68 + 24       : A0 6B 05 39 F6 7F 00 00 B3::f() потомков после B3 нет, здесь адрес функции определенной в B3::f
        // 0x00007FF6390E7D70 + 32       : F0 FF FF FF FF FF FF FF vtbl(B2,DELIMITER1) offset = -16
        // 0x00007FF6390E7D78 + 40       : C0 2D 0E 39 F6 7F 00 00 type_info
        // 0x00007FF6390E7D80 + 48       : B0 69 05 39 F6 7F 00 00
        // 0x00007FF6390E7D88 + 56       : E0 B6 0C 39 F6 7F 00 00
        // 0x00007FF6390E7D90 + 64       : E0 FF FF FF FF FF FF FF vtbl(B1) offset = -32
        // 0x00007FF6390E7D98 + 72       : C0 2D 0E 39 F6 7F 00 00 type_info
        // 0x00007FF6390E7DA0 + 80       : 50 B7 0C 39 F6 7F 00 00 f() - переопределенная в потомке на B3::f поэтому здесь адрес функции переходника, который изменяет this и вызывает B3::f()
        // 0x00007FF6390E7DA8 + 88       : 00 00 00 00 00 00 00 00
        // 0x00007FF6390E7DB0 + 96       : 00 00 00 00 00 00 00 00
        // 0x00007FF6390E7DB8 + 104      : 00 2E 0E 39 F6 7F 00 00
        // 0x00007FF6390E7DC0 + 112      : A0 6C 05 39 F6 7F 00 00
        // 0x00007FF6390E7DC8 + 120      : 90 6C 05 39 F6 7F 00 00
        // 0x00007FF6390E7DD0 + 128      : 00 00 00 00 00 00 00 00
        // 0x00007FF6390E7DD8 + 136      : 10 2E 0E 39 F6 7F 00 00
        // 0x00007FF6390E7DE0 + 144      : 60 6D 05 39 F6 7F 00 00
        // 0x00007FF6390E7DE8 + 152      : 90 6C 05 39 F6 7F 00 00


        uint64_t vtable_B3 = *(reinterpret_cast<uint64_t*>(b3)) - 16;
        utils::dump("vtable_B3", reinterpret_cast<void*>(vtable_B3), 8 * 20, 8, true, true);

        B1 b;
        b.f();

        B1* b1 = b3;
        std::cout << "b3 this " << b3 << "b1 this " << b1 << std::endl;
        b1->f();

        // при вызове фунции через базовый класс, береца адрес функции из таблицы vptr и вызывается
        // если функция базового класса переопределена, то там будет указатель на некую функцию переходник,
        // в которой изменяется(смещается) this и вызывается последняя переопределенная функция.
        // размер смещения this и адрес переопределенной фунции жестко зашиты в этой функции-переходнике
    }
}
