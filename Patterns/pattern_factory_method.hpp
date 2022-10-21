#pragma once

#include <iostream>
#include <memory>

namespace pattern_factory_method {

    enum class Buildings {
        HOUSE,
        CASTLE
    };

    struct BuildingSpec {
        virtual Buildings get_type() const   = 0;
        virtual std::string get_name() const = 0;
    };

    struct HouseSpec : public BuildingSpec {
        HouseSpec(std::string param) :
            house_param(param) {}
        Buildings get_type() const override { return Buildings::HOUSE; }
        std::string get_name() const override { return "House, param: " + house_param; }
        std::string house_param;
    };

    struct CastleSpec : public BuildingSpec {
        CastleSpec(std::string param) :
            castle_param(param) {}
        Buildings get_type() const override { return Buildings::CASTLE; }
        std::string get_name() const override { return "Castle, param: " + castle_param; }
        std::string castle_param;
    };

    class Building {
    public:
        Building(std::string name_) :
            name(name_) {}
        virtual ~Building() {}
        std::string name;
    };

    class House : public Building {
    public:
        House() :
            Building("House") {}
        House(const HouseSpec* spec) :
            Building(spec->get_name()) {
            // spec-> ...
        }
    };

    class Castle : public Building {
    public:
        Castle() :
            Building("Castle") {}
        Castle(const CastleSpec* spec) :
            Building(spec->get_name()) {
            // spec-> ...
        }
    };

    std::unique_ptr<Building> MakeBuilding(Buildings type);
    std::unique_ptr<Building> MakeBuildingSpec(const BuildingSpec* spec);
}