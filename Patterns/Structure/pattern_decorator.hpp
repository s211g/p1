#pragma once

#include <string>

namespace pattern_decorator {

    class ComponentInterface {
    public:
        virtual std::string GetText() = 0;
    };
}