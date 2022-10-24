#pragma once

#include <string>
#include <vector>

namespace pattern_composite {

    class Equipment {
    public:
        virtual ~Equipment() {}
        std::string Name() const { return name; }
        virtual int Power() = 0;
        virtual std::string Description() const { return name; }
        virtual const Equipment* GetItemByName(std::string name_) {
            if (name_ == name)
                return this;
            return nullptr;
        }

        Equipment(std::string name_) :
            name(name_) {}

    private:
        std::string name;
    };

    class ItemEquipment : public Equipment {
    public:
        virtual ~ItemEquipment() = default;
        ItemEquipment(std::string name_, int power_) :
            Equipment(name_), power(power_) {}

        int Power() override {
            return power;
        }

    private:
        int power;
    };

    class CompositeEquipment : public Equipment {
    public:
        virtual ~CompositeEquipment() {}
        int Power() override {
            int power{0};
            for (const auto& e : equipment) {
                power += e->Power();
            }
            return power;
        }

        std::string Description() const override {
            std::string description = Name() + "(";
            bool first              = true;
            for (const auto& e : equipment) {
                description += (first ? "" : ",") + e->Description();
                if (first)
                    first = !first;
            }
            description += ")";
            return description;
        }

        const Equipment* GetItemByName(std::string name_) override {
            for (const auto& e : equipment) {
                auto item = e->GetItemByName(name_);
                if (item)
                    return item;
            }
            return Equipment::GetItemByName(name_);
        }

        void Add(Equipment* e) {
            equipment.push_back(e);
        };

        CompositeEquipment(std::string name) :
            Equipment(name) {}

    private:
        std::vector<Equipment*> equipment;
    };
}