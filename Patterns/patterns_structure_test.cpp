#include <iostream>

#include <fstream>
#include <filesystem>

#include "pattern_adapter.hpp"
#include "pattern_bridge.hpp"
#include "pattern_composite.hpp"
#include "pattern_decorator.hpp"

namespace patterns_structure_test {

    void test_adapter() {
        std::cout << "\ntest adapter" << std::endl;

        using namespace pattern_adapter;

        std::cout << "\ntest 1" << std::endl;
        // Адаптер класса
        // для адаптации используется множественное неследование
        // наследование по одной ветке, реализация по другой

        class A {
        public:
            void f3() { std::cout << "Target::f3()" << std::endl; }
            void f4() { std::cout << "Target::f4()" << std::endl; }
        };

        class Target : public AdapterInterface1, private A {
        public:
            void f1() override {
                std::cout << "adapted A" << std::endl;
                A::f3();
            };
            void f2() override {
                std::cout << "adapted A" << std::endl;
                A::f3();
            };
        };

        Target t;
        t.f1();
        t.f2();

        std::cout << "\ntest 2" << std::endl;
        // Адаптер объекта
        // для адаптации используется композиция
        // может работать со множеством объектов
        // может использоваться в качестве манипулятора объекта

        using AdaptInterface = AdapterInterface2<A>;
        class Manipulator : public AdaptInterface {
        public:
            Manipulator(A* a) :
                AdaptInterface(a) {}
            void f1() override {
                std::cout << "Adapt+" << std::endl;
                obj->f3();
            }
            void f2() override {
                std::cout << "Adapt+" << std::endl;
                obj->f4();
            }

            void manipulate() {
                std::cout << "manipulate obj:" << std::endl;
                obj->f4();
                obj->f3();
            }
        };

        A a;
        Manipulator man(&a);
        man.f1();
        man.f2();
        man.manipulate();
    }

    using namespace pattern_bridge;

    class FileFlusher : public FlusherInterface {
    public:
        void Open(std::string command) override { file.open(command); }
        void Close() override { file.close(); }
        void Write(std::string text) override { file << text; };

    private:
        std::ofstream file;
    };

    class ConsoleFlusher : public FlusherInterface {
    public:
        void Open(std::string command) override {}
        void Close() override { std::cout << std::flush; }
        void Write(std::string text) override { std::cout << text; };
    };

    FlusherInterface* FlusherFabric(FlusherType type) {
        switch (type) {
            case FlusherType::FILE:
                return new FileFlusher();
            case FlusherType::CONSOLE:
                return new ConsoleFlusher();
            default:
                return nullptr;
        }
    }

    void test_bridge() {
        std::cout << "\ntest bridge" << std::endl;

        // назначение:
        // отделить абстракцию от реализации
        // реализация может быть скрыта от клиента
        // реализацию можно поменять динамически
        // абстракция не обязательно должна иметь одинаковый интерфейс с реализацией

        std::cout << "\ntest 1" << std::endl;
        FlushBrige fb_console(&FlusherFabric, FlusherType::CONSOLE);
        fb_console.Open("");
        fb_console.Write("HI\n");
        fb_console.Close();

        std::cout << "\ntest 2" << std::endl;
        std::string filename = "test_brige.txt";
        std::cout << "write to file: " << std::filesystem::current_path().generic_string() << "/"
                  << filename << std::endl;
        FlushBrige fb_file(&FlusherFabric, FlusherType::FILE);
        fb_file.Open(filename);
        fb_file.Write("HI\n");
        fb_file.Close();
    }

    void test_composite() {
        std::cout << "\ntest composite" << std::endl;
        using namespace pattern_composite;

        // назначение:
        // компонует объекты в древовидные структуры для представления иерархий часть-целое.
        // позволяет клиентам единообразно трактовать индивидуальные и составные объекты
        // позволяет группировать мелкие объекты в более крупные которые в свою очередь могут стать основой для еще более крупных
        // ключем к паттерну я вляется абстрактный класс который представляет одновременно и примитивы и контейнеры

        std::cout << "\ntest 1" << std::endl;

        auto computer = new CompositeEquipment("computer");
        computer->Add(new ItemEquipment("Supply", 10));

        auto mainboard = new CompositeEquipment("Mainboard");
        mainboard->Add(new ItemEquipment("Memory8Gb", 10));
        mainboard->Add(new ItemEquipment("Memory16Gb", 20));
        mainboard->Add(new ItemEquipment("VideoCard", 100));
        computer->Add(mainboard);

        auto pciex_raid = new CompositeEquipment("Raid");
        pciex_raid->Add(new ItemEquipment("Hdd_1TB", 10));
        pciex_raid->Add(new ItemEquipment("Hdd_2TB", 20));
        mainboard->Add(pciex_raid);

        std::cout << "computer description:" << std::endl;
        std::cout << computer->Description() << std::endl;
        std::cout << "computer power consumption: " << computer->Power() << " Watt" << std::endl;

        auto FindByName = [&](std::string name) {
            auto x = computer->GetItemByName(name);
            if (x)
                std::cout << "item type: " << (dynamic_cast<const CompositeEquipment*>(x) ? "Composite" : "Item")
                          << ", description: " << x->Description() << std::endl;
            else
                std::cout << "item " << name << " not found" << std::endl;
        };

        FindByName("Raid");
        FindByName("Raid2");
        FindByName("Memory8Gb");
    }

    void test_decorator() {
        std::cout << "\ntest decorator" << std::endl;
        using namespace pattern_decorator;

        // decorator(wrapper)
        // назначение:
        // динамически добавляет новые обязанности, как альтернатива порождению подклассов

        class Component : public ComponentInterface {
        public:
            Component(std::string text_) :
                text(text_) {}
            std::string GetText() override { return text; }

        private:
            std::string text;
        };

        class ComponentDecoratorPrefix : public ComponentInterface {
        public:
            ComponentDecoratorPrefix(ComponentInterface* obj_) :
                obj(obj_) {}
            std::string GetText() override { return "prefix_" + obj->GetText(); }

        private:
            ComponentInterface* obj;
        };

        class ComponentDecoratorSuffix : public ComponentInterface {
        public:
            ComponentDecoratorSuffix(ComponentInterface* obj_) :
                obj(obj_) {}
            std::string GetText() override { return obj->GetText() + "_suffix"; }

        private:
            ComponentInterface* obj;
        };

        std::cout << "\ntest 1" << std::endl;
        auto component   = new Component("Component");
        auto d_component = new ComponentDecoratorPrefix(new ComponentDecoratorSuffix(component));
        std::cout << "component text: " << d_component->GetText() << std::endl;
    }
}