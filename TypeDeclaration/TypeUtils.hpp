#pragma once

#include <string>
#include <type_traits>
#include <functional>

namespace type_utils {

    template <typename T>
    std::string type2name(T) {
        if (std::is_same_v<T, int>)
            return "int";
        if (std::is_same_v<T, int&>)
            return "int&";
        if (std::is_same_v<T, int*>)
            return "int*";
        if (std::is_same_v<T, std::reference_wrapper<int>>)
            return "ref<int>";
        if (std::is_same_v<T, std::string>)
            return "string";
        if (std::is_same_v<T, std::string&>)
            return "string&";
        return "?";
    }

    template <typename T>
    std::string type2name() {
        if (std::is_same_v<T, int>)
            return "int";
        if (std::is_same_v<T, const int>)
            return "const int";
        if (std::is_same_v<T, int&>)
            return "int&";
        if (std::is_same_v<T, int*&>)
            return "int*&";
        if (std::is_same_v<T, const int*&>)
            return "const int*&";
        if (std::is_same_v<T, int&&>)
            return "int&&";
        if (std::is_same_v<T, const int&>)
            return "const int&";
        if (std::is_same_v<T, int*>)
            return "int*";
        if (std::is_same_v<T, const int*>)
            return "const int*";
        if (std::is_same_v<T, std::reference_wrapper<int>>)
            return "ref<int>";
        if (std::is_same_v<T, std::string>)
            return "string";
        if (std::is_same_v<T, std::string&>)
            return "string&";
        if (std::is_same_v<T, void(int)>)
            return "void(int)";
        if (std::is_same_v<T, void (*)(int)>)
            return "void(*)(int)";
        if (std::is_same_v<T, void (&)(int)>)
            return "void(&)(int)";
        return "?";
    }

}
