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
        virtual void f2() {}
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
        virtual void f2() override {}
    };

    void test1() {
        std::cout << "\ntest1" << std::endl;

        Parent* p    = new Parent();
        Derived1* d1 = new Derived1();
        Derived2* d2 = new Derived2();

        utils::dump("Parent", p, sizeof(Parent), 8, true);
        utils::dump("Derived1", d1, sizeof(Derived1), 8, true);
        utils::dump("Derived2", d2, sizeof(Derived2), 8, true);

        uint64_t vtable_parent   = *(reinterpret_cast<uint64_t*>(p)) - 16;
        uint64_t vtable_derived1 = *(reinterpret_cast<uint64_t*>(d1)) - 16;
        uint64_t vtable_derived2 = *(reinterpret_cast<uint64_t*>(d2)) - 16;

        utils::dump("vtable_parent", reinterpret_cast<void*>(vtable_parent), 8 * 8, 8, true, true);
        utils::dump("vtable_derived1", reinterpret_cast<void*>(vtable_derived1), 8 * 8, 8, true, true);
        utils::dump("vtable_derived2", reinterpret_cast<void*>(vtable_derived2), 8 * 12, 8, true, true);

        Parent* parent_derived2 = d2;
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
        // 0x00007FF6F8436B20 + 0        : 00 00 00 00 00 00 00 00
        // 0x00007FF6F8436B28 + 8        : 40 1D 43 F8 F6 7F 00 00
        // 0x00007FF6F8436B30 + 16       : C0 67 3A F8 F6 7F 00 00
        // 0x00007FF6F8436B38 + 24       : 70 69 3A F8 F6 7F 00 00
        // 0x00007FF6F8436B40 + 32       : E8 FF FF FF FF FF FF FF
        // 0x00007FF6F8436B48 + 40       : 40 1D 43 F8 F6 7F 00 00
        // 0x00007FF6F8436B50 + 48       : E0 68 3A F8 F6 7F 00 00 Foo, тотже что и у derived1
        // 0x00007FF6F8436B58 + 56       : 40 68 3A F8 F6 7F 00 00 FooNotOverridden, тот же что и у derived1
        // 0x00007FF6F8436B60 + 64       : C0 68 3A F8 F6 7F 00 00 f1, тот же что и у derived1
        // 0x00007FF6F8436B68 + 72       : 70 B2 41 F8 F6 7F 00 00 f2, переопределили
        // 0x00007FF6F8436B70 + 80       : 00 00 00 00 00 00 00 00
        // 0x00007FF6F8436B78 + 88       : A0 1D 43 F8 F6 7F 00 00

        // parent_derived2(0x196ae4c6f98) size = 16 :
        // 0x00000196AE4C6F98  : 50 6B 43 F8 F6 7F 00 00
        // 0x00000196AE4C6FA0  : 01 00 00 00 00 00 00 00

        // vtable_parent_derived2(0x7ff6f8436b40) size = 64 :
        // 0x00007FF6F8436B40 + 0        : E8 FF FF FF FF FF FF FF -24 смещение на начало созданного объекта
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
    }
}