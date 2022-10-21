#pragma once

#include "pattern_factory_method.hpp"

namespace pattern_abstract_factory {

    class AbstractProductAInterface {
    public:
        virtual void fn1() = 0;
        virtual void fn2() = 0;
    };

    class AbstractProductBInterface {
    public:
        virtual void fn3() = 0;
        virtual void fn4() = 0;
    };

    class AbstractFactory {
    public:
        virtual AbstractProductAInterface* CreateProductA() = 0;
        virtual AbstractProductBInterface* CreateProductB() = 0;
    };

    // конкретизация продуктов
    class ProductA1 : public AbstractProductAInterface {
    public:
        void fn1() override { std::cout << "ProductA1::fn1()" << std::endl; };
        void fn2() override { std::cout << "ProductA1::fn2()" << std::endl; };
    };

    class ProductB1 : public AbstractProductBInterface {
    public:
        void fn3() override { std::cout << "ProductB1::fn3()" << std::endl; };
        void fn4() override { std::cout << "ProductB1::fn4()" << std::endl; };
    };

    // конкретизация фабрики

    class Factory1 : public AbstractFactory {
    public:
        AbstractProductAInterface* CreateProductA() override { return new ProductA1(); }
        AbstractProductBInterface* CreateProductB() override { return new ProductB1(); }
    };

}