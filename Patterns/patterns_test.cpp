#include <iostream>
#include "patterns_test.hpp"
#include "pattern_factory_method.hpp"
#include "pattern_abstract_factory.hpp"
#include "pattern_builder.hpp"

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
}