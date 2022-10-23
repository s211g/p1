#include <iostream>
#include <cassert>
#include "patterns_test.hpp"
#include "pattern_factory_method.hpp"
#include "pattern_abstract_factory.hpp"
#include "pattern_builder.hpp"
#include "pattern_singleton.hpp"
#include "pattern_prototype.hpp"

namespace patterns_test {

    void test_fabric() {
        using namespace pattern_factory_method;

        std::cout << "\ntest fabric" << std::endl;

        // фабричный медод через производный класс
        std::cout << "\ntest 0" << std::endl;
        CreatorCastle cc;
        std::cout << cc.CreateBuilding()->name << std::endl;

        // через шаблонный класс
        TemlateCreator<House> tch;
        std::cout << tch.CreateBuilding()->name << std::endl;

        // параметризированный фабричный метод
        std::cout << "\ntest 1" << std::endl;
        std::cout << MakeBuilding(Buildings::HOUSE)->name << std::endl;
        std::cout << MakeBuilding(Buildings::CASTLE)->name << std::endl;

        std::cout << "\ntest 2" << std::endl;
        HouseSpec hspec1("hspec1");
        std::cout << MakeBuildingSpec(&hspec1)->name << std::endl;
        HouseSpec hspec2("hspec2");
        std::cout << MakeBuildingSpec(&hspec2)->name << std::endl;
        CastleSpec cspec1("cspec1");
        std::shared_ptr<Building> b = MakeBuildingSpec(&cspec1);
        std::cout << b->name << std::endl;

        std::cout << "\ntest 3" << std::endl;
        Castle::Register();
        House::Register();
        std::cout << Building::MakeBuilding(&hspec1)->name << std::endl;
        std::cout << Building::MakeBuilding(&hspec2)->name << std::endl;
        std::cout << Building::MakeBuilding(&cspec1)->name << std::endl;
    }

    void test_abstract_factory() {
        using namespace pattern_factory_method;

        std::cout << "\ntest abstract factory" << std::endl;

        std::cout << "\ntest 0" << std::endl;
        using namespace pattern_abstract_factory;
        Factory1 f1;

        // клиент пользуется интерфейсом абстрактной фабрики
        // и в действительности может быть изолирован от информации что будет создавать эта фабрика
        AbstractFactory* af = &f1;
        auto pa1            = af->CreateProductA();
        auto pb1            = af->CreateProductB();
        pa1->fn1();
        pa1->fn2();
        pb1->fn3();
        pb1->fn4();
    }

    void test_builder() {
        using namespace pattern_builder;

        class ResultCreaterT1 {
        public:
            ResultBase* CreateResult(BuilderBase& builder) {
                builder.Init();
                builder.BuildPartA(1);
                builder.BuildPartA(2);
                builder.BuildPartA(3);
                builder.BuildPartB(4);
                builder.BuildPartB(5);
                return builder.GetResult();
            }
        };

        std::cout << "\ntest builder" << std::endl;
        ResultCreaterT1 rc;
        BuilderT1 bt1;
        ResultBase* result = rc.CreateResult(bt1);
        result->print();
    }

    void test_singleton() {
        using namespace pattern_singleton;

        std::cout << "\ntest singleton" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        std::cout << Singleton1::Instance() << std::endl;
        Singleton1::Instance()->f();
        std::cout << Singleton1::Instance() << std::endl;
        Singleton1::Instance()->f();

        std::cout << "\ntest 2" << std::endl;
        std::cout << Singleton2::Instance() << std::endl;
        Singleton2::Instance()->f();
        std::cout << Singleton2::Instance() << std::endl;
        Singleton2::Instance()->f();


        std::cout << "\ntest 3" << std::endl;
        class A {
        public:
            A() { std::cout << "A::A()" << std::endl; }
            A(int, double) { std::cout << "A::A(int, double)" << std::endl; }
            ~A() { std::cout << "A::~A()" << std::endl; }
            void f() { std::cout << "void A::f()" << std::endl; }
        };
        std::cout << Singleton_t<A>::getInstance() << std::endl;
        Singleton_t<A>::getInstance()->f();
        std::cout << Singleton_t<A>::getInstance() << std::endl;
        Singleton_t<A>::getInstance()->f();

        std::cout << "\ntest 4" << std::endl;
        class A4 {
        public:
            A4() { std::cout << "A4::A4()" << std::endl; }
            A4(int, double) { std::cout << "A4::A4(int, double)" << std::endl; }
            ~A4() { std::cout << "A4::~A4()" << std::endl; }
            void f() { std::cout << "void A4::f()" << std::endl; }
        };
        using S4 = Singleton_t<A4>;

        std::cout << S4::getInstanceInit(1, 1.1) << std::endl;
        S4::getInstance()->f();
        std::cout << S4::getInstance() << std::endl;
        S4::getInstance()->f();

        std::cout << "---" << std::endl;
    }

    void test_prototype() {
        std::cout << "\ntest prototype" << std::endl;

        using namespace pattern_prototype;

        class A {
        public:
            int i{0};
            double d{1.1};
            long long l{100};
            A() { std::cout << "A::A()" << std::endl; }
            A(int i_, double d_) :
                i(i_), d(d_) {
                std::cout << "A::A(int, double)" << std::endl;
            }

            bool Init(long long l_) {
                l = l_;
                return true;
            }

            ~A() { std::cout << "A::~A()" << std::endl; }
            void f() { std::cout << "void A::f() i = " << i << ", d = " << d << ", l = " << l << std::endl; }
        };


        std::cout << "\ntest 1" << std::endl;
        Prototype<A> a;
        a.Reset(1, 2.2);
        a.Initialize(&A::Init, 100);
        auto ca1 = a.Clone();
        ca1->f();
        auto ca2 = a.Clone();
        ca2->f();
    }
}