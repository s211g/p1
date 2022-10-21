#include "pattern_factory_method.hpp"

namespace pattern_factory_method {

    std::unique_ptr<Building>
    MakeBuilding(Buildings type) {
        switch (type) {
            case Buildings::HOUSE:
                return std::make_unique<House>();
            case Buildings::CASTLE:
                return std::make_unique<Castle>();
            default:
                return nullptr;
        }
    }

    std::unique_ptr<Building> MakeBuildingSpec(const BuildingSpec* spec) {
        switch (spec->get_type()) {
            case Buildings::HOUSE:
                return std::make_unique<House>(static_cast<const HouseSpec*>(spec));
            case Buildings::CASTLE:
                return std::make_unique<Castle>(static_cast<const CastleSpec*>(spec));
            default:
                return nullptr;
        }
    }
}