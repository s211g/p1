#pragma once

#include <iostream>
#include <string>
#include <type_traits>
#include <functional>

namespace utils {

    void dump(std::string caption, void* data, uint32_t size, uint32_t interval = 8, bool print_offset = false, bool print_base_offsest = false);

    class A {
    public:
        int i{0};
        A() { std::cout << "A::A()" << std::endl; }
        A(int i_) :
            i(i_) { std::cout << "A::A(int) i = " << i_ << std::endl; }
        A(A&)       = delete;
        A(const A&) = delete;
        A& operator=(const A&) = delete;
        A& operator=(A&) = delete;
        A(A&&)           = delete;
        A& operator=(A&&) = delete;
    };

    class B {
    public:
        int i{0};
        B() { std::cout << "B::B()" << std::endl; }
        B(int i_) :
            i(i_) { std::cout << "B::B(int) i = " << i_ << std::endl; }
        B(int i_, int j_) :
            i(i_) { std::cout << "B::B(int, int) i = " << i_ << ", j = " << j_ << std::endl; }
        B(B&)       = delete;
        B(const B&) = delete;
        B& operator=(const B&) = delete;
        B& operator=(B&) = delete;
        B(B&&) { std::cout << "B::B(B&&)" << std::endl; }
        B(B&&, int) { std::cout << "B::B(B&&,int)" << std::endl; }
        B& operator=(B&&) = delete;

        void f() {}
    };

    template <typename T>
    std::string type2name(T) {
        if (std::is_same_v<T, int>)
            return "int";
        if (std::is_same_v<T, int&>)
            return "int&";
        if (std::is_same_v<T, int&&>)
            return "int&&";
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
        if (std::is_same_v<T, double>)
            return "double";
        if (std::is_same_v<T, const double>)
            return "const double";
        if (std::is_same_v<T, char>)
            return "char";
        if (std::is_same_v<T, const char>)
            return "const char";
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
        if (std::is_same_v<T, const int&&>)
            return "const int&&";
        if (std::is_same_v<T, const int&>)
            return "const int&";
        if (std::is_same_v<T, int*>)
            return "int*";
        if (std::is_same_v<T, const int*>)
            return "const int*";
        if (std::is_same_v<T, int* const>)
            return "int* const";
        if (std::is_same_v<T, std::reference_wrapper<int>>)
            return "ref<int>";
        if (std::is_same_v<T, std::string>)
            return "string";
        if (std::is_same_v<T, std::string&>)
            return "string&";

        if (std::is_same_v<T, std::initializer_list<int>>)
            return "std::initializer_list<int>";
        if (std::is_same_v<T, std::initializer_list<double>>)
            return "std::initializer_list<double>";

        // function
        if (std::is_same_v<T, int(void)>)
            return "int(void)";
        if (std::is_same_v<T, void(int)>)
            return "void(int)";
        if (std::is_same_v<T, void (*)(int)>)
            return "void(*)(int)";
        if (std::is_same_v<T, void (&)(int)>)
            return "void(&)(int)";

        // array
        if (std::is_same_v<T, int[10]>)
            return "int[10]";
        if (std::is_same_v<T, int(*)[10]>)
            return "int(*)[10]";
        //typedef int m10_t[10];
        //if (std::is_same_v<T, m10_t&>)
        //    return "m10_t& (typedef int m10_t[10];)";
        if (std::is_same_v<T, int(&)[10]>)
            return "int(&)[10]";

        // class
        if (std::is_same_v<T, B>)
            return "B";
        if (std::is_same_v<T, const B>)
            return "const B";
        if (std::is_same_v<T, const B&>)
            return "const B&";
        if (std::is_same_v<T, B*>)
            return "B*";
        if (std::is_same_v<T, B&>)
            return "B&";
        if (std::is_same_v<T, B&&>)
            return "B&&";

        return "?";
    }

    template <typename T>
    constexpr auto toUType(T t) noexcept {
        return static_cast<std::underlying_type_t<T>>(t);
    }
}
