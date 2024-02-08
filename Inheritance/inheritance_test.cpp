#include <iostream>

#include "inheritance_test.hpp"

namespace inheritance_test {

    void dump(std::string caption, uint8_t* data, uint32_t size, uint32_t interval = 8) {
        std::cout << caption << "(" << (void*)data << ") : " << std::endl;
        char buff[23];
        for (int i = 0; i < size; ++i) {
            sprintf(buff, "%02X", (int)data[i]);
            //std::cout << (int)data[i] << " ";
            //std::cout << buff << " ";
            std::cout << ((i && (!(i % interval))) ? "\n" : "") << buff << " ";
        }
        std::cout << std::endl;
    }

    class A {
    public:
        void f1(const A& a) {
            std::cout << "A::f1() a.i_priv = " << a.i_priv << std::endl;
            std::cout << "A::f1() a.i_prot = " << a.i_prot << std::endl;
        }

        virtual void f_v(const A& a){};
        virtual void f_v(){};

    private:
        int i_priv{0};

    protected:
        int i_prot{0};
    };

    class B : public A {
    public:
        void f2(const A& a) {
            // нет доступа из потомка к данным другого объекта
            //std::cout << "B::f2() a.i_priv = " << a.i_priv << std::endl;
            //std::cout << "B::f2(const A& a) a.i_prot = " << a.i_prot << std::endl;
        }

        void f2(const B& b) {
            // есть доступ
            std::cout << "B::f2(const B& b) b.i_prot = " << b.i_prot << std::endl;
            // нет доступа
            //std::cout << "B::f2(const B& b) b.i_priv = " <<  b.i_priv << std::endl;
        }

        void f3() {
            // нет доступа к приватным данным базового класса
            //std::cout << "B::f3() i_priv = " << i_priv << std::endl;

            // есть доступ к защищенным данным базового класса
            std::cout << "B::f3() i_prot = " << i_prot << std::endl;
        }

        virtual void f_v(const A& a) override {
            // у виртуальной функции также нет доступа из потомка к данным другогр объекта
            //std::cout << "B::f2() a.i_priv = " << a.i_priv << std::endl;
            //std::cout << "B::f2() a.i_prot = " << a.i_prot << std::endl;
        }

        virtual void f_v() override {
            // нет доступа к приватным данным базового класса
            //std::cout << "B::f3() i_priv = " << i_priv << std::endl;
            // есть доступ к защищенным данным базового класса
            std::cout << "B::f3() i_prot = " << i_prot << std::endl;
        }
    };

    void test_1() {
        std::cout << "\ntest 1" << std::endl;

        // protected - доступ из иерархии потомков
        // private - доступ только тот уровень на котором переменная(функция определена), потомки не имеют доступа

        // B::f3()
        // функции потомка тогоже объекта имеют доступ только к protected членам

        // B::f1()
        // protected,private !!! объекты класса-потомка имеют доступ только через фунции этого уровня иерархии где объявлена переменная
        // (тоже относится к переопределенным виртуальным ф-м)

        // B::f2()
        // функции объекта класса потомка имеют доступ к приватному члену другого объекта объявленого в базовом классе

        A a1;
        A a2;
        std::cout << "\n---------------------" << std::endl;
        a2.f1(a1);

        B b;
        B b2;
        std::cout << "\n---------------------" << std::endl;
        b.f3();
        std::cout << "\n---------------------" << std::endl;
        b.f2(b2);

        std::cout << "\n---------------------" << std::endl;
        std::cout << "b.f1(const A& a)" << std::endl;
        b.f1(a1);
    }

    class D {
    private:
        int j_priv;
    };

    class C : private A, public D {
    public:
        using A::i_prot;
        //using D::j_priv; // нельзя тк в базовом прописан private

        void f3() {
            // есть доступ, в закрытом наследовании если использовать using
            std::cout << "B::f3() i_prot = " << i_prot << std::endl;
        }
    };

    void test_2() {
        std::cout << "\ntest 2" << std::endl;

        // используя using можно разприватить закрытое наследование, но нельзя разприватить приватные секции базового класса
        C c;
        c.f3();
    }

    class A3 {
        int i{1};
    };
    class B3 {
        int j{2};
    };
    class C3 : public A3, public B3 {
        int k{3};
    };

    void test_3() {
        std::cout << "\ntest 3" << std::endl;

        C3* c = new C3();
        A3* a = c; // неявно преобразование вниз
        B3* b = c; // неявно преобразование вниз, !!! указатель b указывает на другое место в памяти

        std::cout << " c " << c << std::endl;
        std::cout << " a " << a << std::endl;
        std::cout << " b " << b << std::endl;

        //C3* c3 = b; // неявное преобразование вверх невозможно
        C3* c3 = static_cast<C3*>(b); // можно только явное преобразование вверх
    }

    class A4 {
    public:
        A4() { std::cout << "A4::A4()" << std::endl; }
        A4(int i_) :
            i(i_) { std::cout << "A4::A4(int i)" << std::endl; }
        int i{1};
    };

    class B4A : public A4 {
    public:
        int j{2};
    };

    class C4A : public A4 {
    public:
        int k{3};
    };


    class D4 : public B4A, public C4A {
    public:
        int l{4};
    };

    class B4vA : virtual public A4 {
    public:
        B4vA() :
            A4(9) {
            std::cout << "B4vA::B4vA" << std::endl;
            i = 7;
        }
        int j{2};
    };

    class C4vA : virtual public A4 {
    public:
        C4vA() :
            A4(10) {
            std::cout << "C4vA::C4vA" << std::endl;
            i = 8;
        }
        int k{3};
    };

    class E4 {
    public:
        E4() {
            std::cout << "E4::E4()" << std::endl;
        }
        int m{11};
    };

    class D4v : public E4, public B4vA, public C4vA {
    public:
        int l{4};
    };

    void test_4() {
        std::cout << "\ntest 4" << std::endl;


        std::cout << "non-virtual rhombic inheritance -----------------------------------------------------" << std::endl;

        D4 d;
        dump("d", reinterpret_cast<uint8_t*>(&d), sizeof(d), 4);
        std::cout << std::endl;

        //01 00 00 00 A4 базовый C4A
        //02 00 00 00
        //01 00 00 00 A4 базовый B4A
        //03 00 00 00
        //04 00 00 00

        //d.i = 5;      // неопределенность в каком инстансе менять i C4A или в B4A
        d.C4A::i = 6; // указать какой инстанс менять через область видимости
        B4A* b   = &d;
        b->i     = 5;
        dump("d", reinterpret_cast<uint8_t*>(&d), sizeof(d), 4);
        std::cout << std::endl;

        //05 00 00 00 // b->i = 5;
        //02 00 00 00
        //06 00 00 00 // d.C4A::i = 6;
        //03 00 00 00
        //04 00 00 00

        std::cout << "virtual rhombic inheritance -----------------------------------------------------" << std::endl;

        B4vA bb;
        dump("B4vA bb", reinterpret_cast<uint8_t*>(&bb), sizeof(bb), 4);
        std::cout << std::endl;

        // !!! в данном случае вызывается конструктор базового виртуального класса
        // A4::A4(int i)  - конструктор базового
        // B4vA::B4vA
        // B4vA bb(0x5c359ff560) :
        // 88 07 0F 2E
        // F7 7F 00 00
        // 02 00 00 00
        // 07 00 00 00

        std::cout << "D4v dv; ... " << std::endl;
        D4v dv;
        dump("D4v", reinterpret_cast<uint8_t*>(&dv), sizeof(dv), 4);
        std::cout << std::endl;

        //  !!! Хоть и вызывается конструктор базового класса B4vA() : A4(9) {}
        // Но он не вызывается ))
        // вызывается только конструктор по умолчанию до всех конструкторов с виртуальным базовым классом
        //
        //  Все классы, наследующие виртуальный базовый класс, будут иметь виртуальную таблицу,
        //  даже если в противном случае у них ее обычно не было бы, и, таким образом, размер
        //  экземпляров этих классов будет больше на размер указателя.

        // D4v dv; ...
        // A4::A4()  - вызывается один раз
        // E4::E4()
        // B4vA::B4vA  - не вызывает A4(int), хоть и прописан
        // C4vA::C4vA
        // D4v(0xdc3d7ff970) :
        // 98 07 4B 88   B4vA почемуто переместился в начало, хоть первым идет Е4
        // F7 7F 00 00   B4vA
        // 02 00 00 00   B4vA
        // 0B 00 00 00   E4
        // B0 07 4B 88   C4vA
        // F7 7F 00 00   C4vA
        // 03 00 00 00   C4vA
        // 04 00 00 00   D4v
        // 08 00 00 00   A4 - в конец уехал виртуальный класс
        // 00 00 00 00

        // в начале виртуальных классов указатель на таблицу которая, по сути, сохраняет смещение от каждого подкласса к виртуальному подобъекту

        B4vA* bv = &dv;
        C4vA* cv = &dv;
        A4* a4   = &dv;

        dump("B4vA", reinterpret_cast<uint8_t*>(bv), sizeof(B4vA), 4);
        dump("C4vA", reinterpret_cast<uint8_t*>(cv), sizeof(C4vA), 4);
        dump("A4", reinterpret_cast<uint8_t*>(a4), sizeof(A4), 4);

        // B4vA :
        // E8 F5 16 0F  // ?? указатель на класс A4
        // F6 7F 00 00  // ??
        // 02 00 00 00
        // F9 7F 00 00
        // C4vA :
        // 00 F6 16 0F  // ?? указатель на класс A4
        // F6 7F 00 00  // ??
        // 03 00 00 00
        // 04 00 00 00
        // A4 0xfb9b3ff9c0 :
        // 08 00 00 00
    }
}
