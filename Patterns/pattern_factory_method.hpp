#pragma once

#include <iostream>
#include <memory>
#include <map>

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

    class Building;
    using BuildingFactory_t = std::unique_ptr<Building> (*)(const BuildingSpec* spec);

    class Building {
    public:
        Building(Buildings type_, std::string name_) :
            type(type_), name(name_) {}
        virtual ~Building() {}
        std::string name;
        Buildings type;

    public:
        static void RegisterBuilding(Buildings type, BuildingFactory_t factory) {
            building_registry[type] = factory;
        }

        static std::unique_ptr<Building> MakeBuilding(const BuildingSpec* spec) {
            BuildingFactory_t factory = building_registry[spec->get_type()];
            if (factory)
                return factory(spec);
            return nullptr;
        }

    private:
        static std::map<Buildings, BuildingFactory_t> building_registry;
    };

    class House : public Building {
    public:
        House() :
            Building(Buildings::HOUSE, "House") {}
        House(const HouseSpec* spec) :
            Building(Buildings::HOUSE, spec->get_name()) {
            // spec-> ...
        }

        static std::unique_ptr<Building> MakeBuilding(const BuildingSpec* spec) {
            return std::make_unique<House>(static_cast<const HouseSpec*>(spec));
        }

        static void Register() {
            RegisterBuilding(Buildings::HOUSE, House::MakeBuilding);
        }
    };

    class Castle : public Building {
    public:
        Castle() :
            Building(Buildings::CASTLE, "Castle") {}
        Castle(const CastleSpec* spec) :
            Building(Buildings::CASTLE, spec->get_name()) {
            // spec-> ...
        }

        static std::unique_ptr<Building> MakeBuilding(const BuildingSpec* spec) {
            return std::make_unique<Castle>(static_cast<const CastleSpec*>(spec));
        }

        static void Register() {
            RegisterBuilding(Buildings::CASTLE, Castle::MakeBuilding);
        }
    };

    // создание объекта через тип
    std::unique_ptr<Building> MakeBuilding(Buildings type);
    // создание объекта через спецификатор
    std::unique_ptr<Building> MakeBuildingSpec(const BuildingSpec* spec);

    // работа с пулом:
    // регистрация пула
    // Castle::Register();
    // House::Register();
    // создание объекта через спецификатор из пула зарегистрированных
    // Building::MakeBuilding(const BuildingSpec* spec);
}