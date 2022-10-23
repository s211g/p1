#pragma once

#include <iostream>

namespace pattern_adapter {

    class AdapterInterface1 {
    public:
        virtual ~AdapterInterface1() = default;
        virtual void f1()            = 0;
        virtual void f2()            = 0;
    };

    template <typename T>
    class AdapterInterface2 {
    public:
        virtual ~AdapterInterface2() = default;
        AdapterInterface2(T* obj_) :
            obj(obj_) {}
        virtual void f1() = 0;
        virtual void f2() = 0;

    protected:
        T* obj;
    };

}