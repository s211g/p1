#include <iostream>
#include "TypeUtils.hpp"

#include "template_universal_arguments_test.hpp"

namespace template_universal_arguments_test {

    template <typename T>
    std::string fi0(T x) {
        return type_utils::type2name<T>() + ",           param: " + type_utils::type2name<decltype(x)>();
    }

    template <typename T>
    std::string fi1(T& x) {
        return type_utils::type2name<T>() + ",           param: " + type_utils::type2name<decltype(x)>();
    }

    template <typename T>
    std::string fi2(const T& x) {
        return type_utils::type2name<T>() + ",           param: " + type_utils::type2name<decltype(x)>();
    }

    template <typename T>
    std::string fi3(T* x) {
        return type_utils::type2name<T>() + ",           param: " + type_utils::type2name<decltype(x)>();
    }

    template <typename T>
    std::string fi4(const T* x) {
        return type_utils::type2name<T>() + ",           param: " + type_utils::type2name<decltype(x)>();
    }

    template <typename T>
    std::string fi5(T&& x) {
        return type_utils::type2name<T>() + ",           param: " + type_utils::type2name<decltype(x)>();
    }

    void f_int(int) {}

    int i          = 1;
    const int ci   = 1;
    const int& cis = ci;
    int* pi        = &i;
    const int* cpi = &i;
    int m10[10];

    void test_template_argument() {
        std::cout << "\ntest_template_argument" << std::endl;

        std::cout << "\ntest 0 f(T param)" << std::endl;
        // параметр перетаскивается базовый тип
        // Т - базовый тип
        std::cout << "arg - int,        param type - int,  T: " << fi0(i) << std::endl;
        std::cout << "arg - const int,  param type - int,  T: " << fi0(ci) << std::endl;
        std::cout << "arg - const int&, param type - int,  T: " << fi0(cis) << std::endl;
        std::cout << "arg - int*,       param type - int*, T: " << fi0(pi) << std::endl;
        std::cout << "arg - const int*, param type - int*, T: " << fi0(cpi) << std::endl;
        // для функции:
        std::cout << "arg - void(int),  param type - void(*)(int), T: " << fi0(f_int) << std::endl;
        // массив
        std::cout << "arg - int[10],  param type -               , T: " << fi0(m10) << std::endl;
        // вывод:
        //arg - int,        param type - int,  T: int,           param: int
        //arg - const int,  param type - int,  T: int,           param: int
        //arg - const int&, param type - int,  T: int,           param: int
        //arg - int*,       param type - int*, T: int*,           param: int*
        //arg - const int*, param type - int*, T: const int*,           param: const int*
        //arg - void(int),  param type - void(*)(int), T: void(*)(int),           param: void(*)(int)
        //arg - int[10],  param type -               , T: int*,           param: int*

        std::cout << "\ntest 1 f(T& param)" << std::endl;
        // в параметр перетаскивается базовый тип& и константность если есть
        // Т - базовый тип и константность если есть в аргументе
        std::cout << "arg - int,        param type - int&,       T: " << fi1(i) << std::endl;
        // не подходит сигнатура для временных и X значений
        //std::cout << "arg - int(),        param type - int&,       T: " << fi1(int()) << std::endl;
        //std::cout << "arg - move(int),        param type - int&,       T: " << fi1(std::move(i)) << std::endl;
        // !!! но подходит для const r и const X значений
        std::cout << "arg - move(const int),        param type - ,       T: " << fi1(std::move(ci)) << std::endl;

        std::cout << "arg - const int,  param type - const int&, T: " << fi1(ci) << std::endl;
        std::cout << "arg - const int&, param type - const int&, T: " << fi1(cis) << std::endl;
        std::cout << "arg - int*,       param type - int*&,      T: " << fi1(pi) << std::endl;
        std::cout << "arg - const int*, param type - const int*&,T: " << fi1(cpi) << std::endl;
        // для функции:
        std::cout << "arg - void(int),  param type - void(&)(int), T: " << fi1(f_int) << std::endl;
        // массив
        std::cout << "arg - int[10],  param type -               , T: " << fi1(m10) << std::endl;
        // вывод:
        //arg - int,        param type - int&,       T: int,           param: int&
        //arg - move(const int),        param type - ,       T: const int,           param: const int&
        //arg - const int,  param type - const int&, T: const int,           param: const int&
        //arg - const int&, param type - const int&, T: const int,           param: const int&
        //arg - int*,       param type - int*&,      T: int*,           param: int*&
        //arg - const int*, param type - const int*&,T: const int*,           param: const int*&
        //arg - void(int),  param type - void(&)(int), T: void(int),           param: void(&)(int)
        //arg - int[10],  param type -               , T: int[10],           param: int(&)[10]

        std::cout << "\ntest 2 f(const T& param)" << std::endl;
        // в параметр перетаскивается базовый тип& + добавляется всегда константность
        // Т всегда базовый тип

        std::cout << "arg - int,        param type - const int&, T: " << fi2(i) << std::endl;
        // !!! подходит сигнатура для временных и X значений, сворачиваются в сылку на базовый тип
        std::cout << "arg - int(),        param type - ,       T: " << fi2(int()) << std::endl;
        std::cout << "arg - move(int),        param type - ,       T: " << fi2(std::move(i)) << std::endl;

        std::cout << "arg - int,        param type - const int&, T: " << fi2(i) << std::endl;
        std::cout << "arg - const int,  param type - const int&, T: " << fi2(ci) << std::endl;
        std::cout << "arg - const int&, param type - const int&, T: " << fi2(cis) << std::endl;
        // вывод:
        //arg - int,        param type - const int&, T: int,           param: const int&
        //arg - int(),        param type - ,       T: int,           param: const int&
        //arg - move(int),        param type - ,       T: int,           param: const int&
        //arg - int,        param type - const int&, T: int,           param: const int&
        //arg - const int,  param type - const int&, T: int,           param: const int&
        //arg - const int&, param type - const int&, T: int,           param: const int&t

        std::cout << "\ntest 3 f(T* param)" << std::endl;
        // в параметр перетаскивается базовый тип* и константность если есть
        // Т - базовый тип и константность если есть в аргументе
        std::cout << "arg - int*,        param type - int*,         T: " << fi3(pi) << std::endl;
        std::cout << "arg - const int*,  param type - const int*,   T: " << fi3(cpi) << std::endl;
        // для функции:
        std::cout << "arg - void(int),   param type - void(*)(int), T: " << fi3(f_int) << std::endl;
        // массив
        std::cout << "arg - int[10],     param type - int*,         T: " << fi3(m10) << std::endl;
        // вывод:
        //arg - int*,        param type - int*,         T: int,           param: int*
        //arg - const int*,  param type - const int*,   T: const int,           param: const int*
        //arg - void(int),   param type - void(*)(int), T: void(int),           param: void(*)(int)
        //arg - int[10],     param type - int*,         T: int,           param: int*

        std::cout << "\ntest 4 f(const T* param)" << std::endl;
        // в параметр перетаскивается базовый тип* + добавляется всегда константность
        // Т всегда базовый тип
        std::cout << "arg - int*,        param type - int*,       T: " << fi4(pi) << std::endl;
        std::cout << "arg - const int*,  param type - const int*, T: " << fi4(cpi) << std::endl;
        // вывод:
        //arg - int*,        param type - int*,       T: int,           param: const int*
        //arg - const int*,  param type - const int*, T: int,           param: const int*
    }

    void test_universal_reference_argument() {
        std::cout << "\ntest_universal_reference_argument" << std::endl;

        std::cout << "\ntest 5 f(T&& param)" << std::endl;
        // общее правило template<typename T>f(T&& param) -> f(param_type param):
        // base - базовый тип(int, double, char ...)
        // - в аргументе тип X - для Т = X&, param_type = X& (добавляется ссылочность)
        // - в аргументе тип rvalue(временный объект) -  Т = base, param_type = base&& (добавляется константность если она была в аргументе)
        // - в аргументе тип именованная переменная rvalue(base&&) - выводится как еслиб вызвали f(base) (добавляется константность если она была в аргументе)

        // Для lvalue преобразуется к [константной]ссылке(к ссылке на указатель если в аргументе указатель)
        // Для lvalue ссылка или значение:
        // lvalue по значению и по ссылке:
        // в параметр перетаскивается базовый тип& и константность если есть
        // Т - базовый тип& и константность если есть в аргументе
        // lvalue указатель:
        // в параметр перетаскивается базовый тип*& и константность если есть
        // Т - базовый тип*& и константность если есть в аргументе
        // Для rvalue в аргументе:
        // параметр тип&&
        // Т - базовый тип
        std::cout << "arg - int,         param type - int&,        T: " << fi5(i) << std::endl;
        std::cout << "arg - const int,   param type - const int&,  T: " << fi5(ci) << std::endl;
        std::cout << "arg - const int&,  param type - const int&,  T: " << fi5(cis) << std::endl;
        std::cout << "arg - int rvalue f(123),  param type - int&&,       T: " << fi5(123) << std::endl;
        std::cout << "arg - int*,        param type - int*&,       T: " << fi5(pi) << std::endl;
        std::cout << "arg - const int*,  param type - const int*&, T: " << fi5(cpi) << std::endl;

        // Именованные и неименованные rvalue:

        // на входе ИМЕНОВАННАЯ rvalue ссылка !!! но это не считается rvalue а считается как будто передан в аргумент int
        int&& i5(5);
        const int&& i5c(5);
        std::cout << "\nArg: Named r-value" << std::endl;
        std::cout << "arg - " << type_utils::type2name<decltype(i5)>() << ",  param type - , T: " << fi5(i5) << std::endl;
        std::cout << "arg - " << type_utils::type2name<decltype(i5c)>() << ",  param type - , T: " << fi5(i5c) << std::endl;

        // на входе НЕ ИМЕНОВАННАЯ rvalue ссылка !!! и поэтому в аргумент идет int&& как rvalue
        std::cout << "\nArg: No Named r-value" << std::endl;
        auto retRvalue = []() -> decltype(auto) {
            return std::move(i);
        };
        std::cout << "arg - " << type_utils::type2name<decltype(retRvalue())>() << ",  param type - , T: " << fi5(retRvalue()) << std::endl;

        // на входе НЕ ИМЕНОВАННАЯ const rvalue ссылка !!! и поэтому в аргумент идет const int&& как rvalue
        auto retRvalueC = []() -> const int&& {
            return std::move(i);
        };
        std::cout << "arg - " << type_utils::type2name<decltype(retRvalueC())>() << ",  param type - , T: " << fi5(retRvalueC()) << std::endl;

        // для функции:
        std::cout << "\nArg: function" << std::endl;
        std::cout << "arg - void(int),  param type -             , T: " << fi5(f_int) << std::endl;

        // массив
        //int m10[10];
        std::cout << "\nArg: array" << std::endl;
        std::cout << "arg - int[10],     param type -    ,         T: " << fi5(m10) << std::endl;

        //вывод:
        //arg - int,         param type - int&,        T: int&,           param: int&
        //arg - const int,   param type - const int&,  T: const int&,           param: const int&
        //arg - const int&,  param type - const int&,  T: const int&,           param: const int&
        //arg - int rvalue,  param type - int&&,       T: int,           param: int&&
        //arg - int*,        param type - int*&,       T: int*&,           param: int*&
        //arg - const int*,  param type - const int*&, T: const int*&,           param: const int*&
        //Arg: Named r-value
        //arg - int&&,  param type - , T: int&,           param: int&
        //arg - const int&&,  param type - , T: const int&,           param: const int&
        //Arg: No Named r-value
        //arg - int&&,  param type - , T: int,           param: int&&
        //arg - const int&&,  param type - , T: const int,           param: const int&&
        //Arg: function
        //arg - void(int),  param type -             , T: void(&)(int),           param: void(&)(int)
        //Arg: array
        //arg - int[10],     param type -    ,         T: int(&)[10],           param: int(&)[10]

        // typeid() пока выводит бред, должен показать что это const int&, а показывает int
        std::cout << ">" << typeid(cis).name() << std::endl;
    }

}