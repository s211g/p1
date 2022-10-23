#include <iostream>

#include <fstream>
#include <filesystem>

#include "pattern_adapter.hpp"
#include "pattern_bridge.hpp"

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
}