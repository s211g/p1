#include "virtual_function_test.hpp"

// void operator delete(void* p, size_t size) noexcept {
//     std::cout << "delete " << p << " size " << size << std::endl;
// }

namespace virtual_function_test {

    void dump(uint8_t* data, uint32_t size) {
        char buff[23];
        for (int i = 0; i < size; ++i) {
            sprintf(buff, "%02X", (int)data[i]);
            //std::cout << (int)data[i] << " ";
            //std::cout << buff << " ";
            std::cout << ((i && (!(i % 8))) ? "\n" : "") << buff << " ";
        }
        std::cout << std::endl;
    }

    void test1() {
        std::cout << "virtual function test1" << std::endl;

        std::cout << " sizeof(void*) " << sizeof(void*) << std::endl;
        std::cout << "sizeof C " << sizeof(C) << std::endl;

        C c;
        C* pc = &c;
        B* pb = dynamic_cast<B*>(&c);
        A* pa = dynamic_cast<A*>(&c);

        try {
            auto& sa = dynamic_cast<A&>(c);
            std::cout << "cast ok" << std::endl;
        }
        catch (...) {
            std::cout << "cast failed" << std::endl;
        }

        if (dynamic_cast<BA*>(&c) == nullptr)
            std::cout << "error cast to BA* : return nullptr" << std::endl;

        try {
            auto& sba = dynamic_cast<BA&>(c);
            std::cout << "cast ok" << std::endl;
        }
        catch (std::bad_cast& ex) {
            std::cout << "error cast to BA& : exception std::bad_cast" << std::endl;
        }

        // в случае ошибки
        // каст на указатель возвращает nullptr
        // каст на ссылку генерирует std::bad_cast

        std::cout << " pc " << pc << " pb " << pb << " pa " << pa << std::endl;
        std::cout << " c " << &pc->c << " b " << &pb->b << " a " << &pa->a << std::endl;

        std::cout << "pc->f()" << std::endl;
        pc->f();
        std::cout << "pb->f()" << std::endl;
        pb->f();
        std::cout << "pa->f()" << std::endl;
        pa->f();

        // class C : public B, public A
        // расположение данных внутри объекта класса С :
        // вложенности у базовых классов нет
        /*
        {
        data B 0000000CDFAFF848 pc,pb -> vptrB
               0000000CDFAFF850          b
        data A 0000000CDFAFF858 pa    -> vptrA
               0000000CDFAFF860          a
        data C 0000000CDFAFF868          c      
        }
        */

        // данные идут по порядку наследования слева направо(class 1: public 2,3,4 - данные внутри объекта идут: 2,3,4,1)
        // !!!тела конструкторов вызываются в этойже последовательности 2,3,4,1
        // !!! из конструкторов(и из деструкторов) таблица на указатели не работает, вызываются реализация функций этого класса или базовых
        // в начале кадого блока(данных класса) идет vptr для этого класса
        // данные производного класса идут последними и ему vptr не надоё

        std::cout << "\n CC ---------------------------" << std::endl;
        CC cc;
        B* cc_pb = &cc;
        CC* pcc  = dynamic_cast<CC*>(cc_pb);
        dump(reinterpret_cast<uint8_t*>(pcc), sizeof(CC));
        // class CC : public B, public A
        // 60 31 8D 46 F6 7F 00 00  vptrB
        // 02 00 00 00              B::b = 2
        // FA 7F 00 00
        // 78 31 8D 46 F6 7F 00 00  vptrA
        // 01 00 00 00              A::a = 1
        // 03 00 00 00              CC::c = 3
        // если потомок не имеет виртуальных функций, но в базовых они есть он все равно полиморфный
    }

    void test2() {
        std::cout << "virtual function test2" << std::endl;

        std::cout << " sizeof(void*) " << sizeof(void*) << std::endl;
        std::cout << "sizeof C_BA " << sizeof(C_BA) << std::endl;

        C_BA cba;
        C_BA* pcba = &cba;
        BA* pba    = dynamic_cast<BA*>(&cba);
        A* pa      = dynamic_cast<A*>(&cba);
        std::cout << " pcba " << pcba << " pba " << pba << " pa " << pa << std::endl;
        std::cout << " c " << &pcba->c << " b " << &pba->b << " a " << &pa->a << std::endl;

        //dump(reinterpret_cast<uint8_t*>(pcba), sizeof(cba));
        dump((uint8_t*)pcba, sizeof(cba));

        std::cout << "pcba->f()" << std::endl;
        pcba->f();
        std::cout << "pba->f()" << std::endl;
        pba->f();
        std::cout << "pa->f()" << std::endl;
        pa->f();

        // class BA : public A
        // class C_BA : public BA
        // расположение данных внутри объекта класса BA :
        /*
        {
        class BA { data A  00000014A1F0F6F8 pcba,pba vptrBA,vptrA
                           00000014A1F0F700          a
                   data BA 00000014A1F0F708          b
                 }
                 data CBA  00000014A1F0F710          c 
        }
        */
    }

    void test3() {
        std::cout << "virtual function test3" << std::endl;

        std::cout << " sizeof(void*) " << sizeof(void*) << std::endl;
        std::cout << "sizeof D " << sizeof(D) << std::endl;

        // class D : public A
        // !!! D без виртуальных функций
        D d;
        D* pd = &d;
        A* pa = dynamic_cast<A*>(&d);

        std::cout << " pd " << pd << std::endl;
        std::cout << " pa " << pa << std::endl;

        //dump(reinterpret_cast<uint8_t*>(pcba), sizeof(cba));
        dump((uint8_t*)pd, sizeof(d));

        // class D : public A
        // !!! D без виртуальных функций
        // расположение данных внутри объекта класса D :
        /*
        {
        class D {   B8 2E BB BF F6 7F 00 00  000000DFB22FF688 pd,pa, vptrD,vptrA
                    01 00 00 00 CC CC CC CC  a
                    04 00 00 00 CC CC CC CC  d
                }
        }
        */
    }

    void test4() {
        std::cout << "virtual function test4" << std::endl;

        std::cout << " sizeof(void*) " << sizeof(void*) << std::endl;
        std::cout << "sizeof F " << sizeof(F) << std::endl;

        //  class F : public E
        // !!! E без виртуальных функций, F c
        F f;
        F* pf = &f;
        E* pe = dynamic_cast<E*>(&f);

        std::cout << " pf " << pf << " &pf->ff " << &pf->ff << std::endl;
        std::cout << " pe " << pe << " &pe->e " << &pe->e << std::endl;

        //dump(reinterpret_cast<uint8_t*>(pcba), sizeof(cba));
        dump((uint8_t*)pf, sizeof(f));

        //sizeof F 16
        // pf 000000873F92FAF8 &pf->ff 000000873F92FB04
        // pe 000000873F92FB00 &pe->e 000000873F92FB00
        // D0 2E C3 E8 F6 7F 00 00
        // 05 00 00 00 06 00 00 00

        //  class F : public E
        // !!! E без виртуальных функций, F c
        // расположение данных внутри объекта класса F :
        /*
        {
        class F {   D0 2E C3 E8 F6 7F 00 00  000000873F92FAF8 pf,vptrF
                    05 00 00 00              000000873F92FB00 e,pe (у E нет vptr)
                    06 00 00 00              000000873F92FB04 f
                }
        }
        */
    }

    void test5() {
        std::cout << "virtual function test5" << std::endl;
        BA ba;
        A* pa = dynamic_cast<A*>(&ba);
        pa->getThis();
        // !!! вызовется переопределенная BA::getThis() хотя сигнатуры виртуальных функций разные
        // случай, когда возвращаемый тип метода - это указатель на сам класс
        pa->A::getThis();
        // можно напрямую указать какой метод вызывать
    }

    class A_NO_VIRTUAL_DESTRUCTOR {
    public:
        int a{1};
        A_NO_VIRTUAL_DESTRUCTOR() {
            std::cout << "A_NO_VIRTUAL_DESTRUCTOR::A_NO_VIRTUAL_DESTRUCTOR()" << std::endl;
        }
        ~A_NO_VIRTUAL_DESTRUCTOR() { std::cout << "A_NO_VIRTUAL_DESTRUCTOR::~A_NO_VIRTUAL_DESTRUCTOR()" << std::endl; }
        virtual void f() {}
    };

    class B_NO_VIRTUAL_DESTRUCTOR {
    public:
        int B{1};
        B_NO_VIRTUAL_DESTRUCTOR() {
            std::cout << "B_NO_VIRTUAL_DESTRUCTOR::B_NO_VIRTUAL_DESTRUCTOR()" << std::endl;
        }
        ~B_NO_VIRTUAL_DESTRUCTOR() { std::cout << "B_NO_VIRTUAL_DESTRUCTOR::~B_NO_VIRTUAL_DESTRUCTOR()" << std::endl; }
        virtual void f() {}
    };

    class C_BNVC_ANVC : public B_NO_VIRTUAL_DESTRUCTOR, public A_NO_VIRTUAL_DESTRUCTOR {
    public:
        int c{1};
        C_BNVC_ANVC() {
            std::cout << "C_BNVC_ANVC::C_BNVC_ANVC()" << std::endl;
        }
        virtual ~C_BNVC_ANVC() { std::cout << "C_BNVC_ANVC::~C_BNVC_ANVC()" << std::endl; }
    };

    void test_virtual_destructor() {
        std::cout << "test_virtual_destructor" << std::endl;

        // раскоментировать в шапке:
        // void operator delete(void* p, size_t size) noexcept {

        std::cout << "\nclass C : public B, public A { ... };" << std::endl;
        {
            C* pc = new C();
            B* pb = pc;
            A* pa = pc;

            std::cout << "pc " << pc << std::endl;
            std::cout << "pb " << pb << std::endl;
            std::cout << "pa " << pa << std::endl;

            // удаление по указателю на родительский класс, все классы полиморфны и имеют виртуальный деструктор
            std::cout << "delete pc" << std::endl;
            delete pc;
            // pc 0x27616b06c10
            // pb 0x27616b06c10
            // pa 0x27616b06c20
            // pc 0x27616b06c10
            // delete pc
            // C::~C()
            // A::~A()
            // B::~B()
            // delete 0x27616b06c10 size 32
        }

        std::cout << "\nclass C_BNVC_ANVC : public B_NO_VIRTUAL_DESTRUCTOR, public A_NO_VIRTUAL_DESTRUCTOR {...}" << std::endl;
        {
            C_BNVC_ANVC* pc             = new C_BNVC_ANVC();
            A_NO_VIRTUAL_DESTRUCTOR* pa = pc;
            B_NO_VIRTUAL_DESTRUCTOR* pb = pc;

            std::cout << "pc " << pc << std::endl;
            std::cout << "pb " << pb << std::endl;
            std::cout << "pa " << pa << std::endl;
            // вывод
            // class C_BNVC_ANVC : public B_NO_VIRTUAL_DESTRUCTOR, public A_NO_VIRTUAL_DESTRUCTOR {...}
            // B_NO_VIRTUAL_DESTRUCTOR::B_NO_VIRTUAL_DESTRUCTOR()
            // A_NO_VIRTUAL_DESTRUCTOR::A_NO_VIRTUAL_DESTRUCTOR()
            // C_BNVC_ANVC::C_BNVC_ANVC()
            // pc 0x1d7682b6f80
            // pb 0x1d7682b6f80
            // pa 0x1d7682b6f90

            // удаление по указателю на базовый класс pa, полиморфный, но деструктор не виртуальный
            std::cout << "delete pa" << std::endl;
            delete pa;
            // вывод:
            // delete pa
            // A_NO_VIRTUAL_DESTRUCTOR::~A_NO_VIRTUAL_DESTRUCTOR()
            // delete 0x1d7682b6f90 size 16 -  ни адрес, ни размер не тот

            // удаление по указателю на базовый класс pb, полиморфный, но деструктор не виртуальный
            std::cout << "delete pb" << std::endl;
            delete pb;
            // вывод:
            // delete pb
            // B_NO_VIRTUAL_DESTRUCTOR::~B_NO_VIRTUAL_DESTRUCTOR()
            // delete 0x1d7682b6f80 size 16  - адрес правильный, размер не тот

            // При удалении через указатель на базовый класс, даже если тип полиморфный
            // не только не вызываются деструкторы в правильной иерархии начиная с родителя
            // но и удалятеся не вся память выделеная в производном классе
            // удаляется не по тому адресу и не того размера
        }
    }

    class J {
    public:
        virtual void f() { std::cout << "J::f()" << std::endl; }
    };

    template <typename T>
    class K : public J {
    public:
        void f() override { std::cout << "K::f()" << std::endl; }
    };

    void test_template_class_and_virtual_function() {
        std::cout << "test_template_class_and_virtual_function" << std::endl;

        J* j = new K<int>();
        j->f();
        // вывод:
        // K::f()

        // в шаблонном классе можно переопределить виртуальную фунцию базового нешаблонного класса
    }

    // стирание типа -----------------------------------------------------------------------------

    class DeleterWrpBase {
    public:
        virtual ~DeleterWrpBase()    = default;
        virtual void Delete(void* p) = 0;
    };

    template <typename Deleter>
    class DeleterWrp : public DeleterWrpBase {
    public:
        DeleterWrp(Deleter d_) :
            d(d_) {}
        virtual void Delete(void* p) { d(p); }; // если делетер знает тип и сам внутри приведет к нужному типу
        Deleter d;
    };

    template <typename T>
    class SmartPtr {
    public:
        template <typename Deleter>
        SmartPtr(T* v_, Deleter d) :
            deleter_wrp(new DeleterWrp<Deleter>(d)),
            v(v_) {
        }

        ~SmartPtr() {
            deleter_wrp->Delete(v);
        }
        DeleterWrpBase* deleter_wrp;
        T* v;
    };

    template <typename T>
    class SmartPtr2 {
    public:
        template <typename Deleter>
        SmartPtr2(T* v_, Deleter d) :
            deleter_wrp(new DeleterWrp2<Deleter>(d)),
            v(v_) {
        }

        ~SmartPtr2() {
            deleter_wrp->Delete(v);
        }

        template <typename Deleter>
        class DeleterWrp2 : public DeleterWrpBase {
        public:
            DeleterWrp2(Deleter d_) :
                d(d_) {}
            virtual void Delete(void* p) { d(static_cast<T*>(p)); }; // реализация внутри класса позволяет привести к нужному типу перед удалением
            Deleter d;
        };

        DeleterWrpBase* deleter_wrp;
        T* v;
    };


    void test_type_erase() {
        std::cout << "test_type_erase" << std::endl;

        int x;
        //SmartPtr<int> ptr_int(new int(123), x);

        {
            SmartPtr<int> ptr_int(new int(123),
                                  [](void* v) {
                                      int* p_int = reinterpret_cast<int*>(v);
                                      std::cout << "try delete void* -> int*, value: " << *p_int << std::endl;
                                      delete p_int;
                                  });
        }
        // вывод:
        // try delete void* -> int*, value: 123

        {
            SmartPtr2<int> ptr_int2(new int(456),
                                    [](int* v) {
                                        int* p_int = reinterpret_cast<int*>(v);
                                        std::cout << "try delete int*, value: " << *p_int << std::endl;
                                        delete p_int;
                                    });
        }
        // вывод:
        // try delete int*, value: 456
    }
}