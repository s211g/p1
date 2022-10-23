#include <iostream>

#include "pattern_adapter.hpp"

namespace patterns_structure_test {

    void test_adapter() {
        using namespace pattern_adapter;

        std::cout << "\ntest adapter" << std::endl;

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

}