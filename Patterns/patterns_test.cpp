#include <iostream>
#include "patterns_test.hpp"
#include "pattern_factory_method.hpp"

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
}