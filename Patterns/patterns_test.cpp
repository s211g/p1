#include <iostream>
#include "patterns_test.hpp"
#include "pattern_factory_method.hpp"

namespace patterns_test {

    void test_fabric() {
        using namespace pattern_factory_method;

        std::cout << "\ntest fabric" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        std::cout << MakeBuilding(Buildings::HOUSE)->name << std::endl;
        std::cout << MakeBuilding(Buildings::CASTLE)->name << std::endl;

        std::cout << "\ntest 2" << std::endl;
        HouseSpec hspec1("hspec1");
        std::cout << MakeBuildingSpec(&hspec1)->name << std::endl;
        HouseSpec hspec2("hspec2");
        std::cout << MakeBuildingSpec(&hspec2)->name << std::endl;
        CastleSpec cspec1("cspec1");
        std::cout << MakeBuildingSpec(&cspec1)->name << std::endl;
    }
}