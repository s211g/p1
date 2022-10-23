#include "pattern_singleton.hpp"

namespace pattern_singleton {
    Singleton1* Singleton1::instance = nullptr;
    std::mutex Singleton1::mtx;

    Singleton1* Singleton1::Instance() {
        if (!instance) {
            std::lock_guard lock(mtx);
            if (instance)
                return instance;
            instance = new Singleton1;
        }
        return instance;
    }
}