#include <iostream>
#include <functional>
#include "typedeclaration_test.hpp"
#include "TypeUtils.hpp"

namespace typedeclaration_test {

    class A {
    public:
        A() { std::cout << "A::A()" << std::endl; }
        ~A() { std::cout << "A::~A()" << std::endl; }
        A(int i) { std::cout << "A::A(int)" << std::endl; }
        int fn(int i) {
            std::cout << "A::fn(" << i << ")" << std::endl;
            return 0;
        }
    };

    void test_array() {
        std::cout << "\ntest array" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        int a1[10];
        int a2[10];
        swap_array(a1, a2);

        std::cout << "\ntest 2" << std::endl;
        A* a = new A[10];
        //delete a; //- assert
        delete[] a;

        std::cout << "\ntest 3" << std::endl;
        int* i = new int[10];
        delete i; // no asserts ???
        std::cout << "---" << std::endl;

        std::cout << "\ntest 4" << std::endl;
        int i4[10];
        std::cout << "sizeof i4 = " << sizeof i4 << std::endl;
        std::cout << "sizeof A = " << sizeof A << std::endl;
    }

    int fn1(int i, double d) {
        std::cout << "fn1() " << i << " " << d << std::endl;
        return 0;
    }
    int fn2(int i, double d) {
        std::cout << "fn2() " << i << " " << d << std::endl;
        return 0;
    }
    int fn3(int i, double d) {
        std::cout << "fn3() " << i << " " << d << std::endl;
        return 0;
    }

    void test_ptr_to_fn() {
        std::cout << "\ntest fn ptr" << std::endl;

        std::cout << "\ntest pointer to fn" << std::endl;
        // ptr to fn
        // тип (*имя_указателя) (параметры);
        int (*p_fn)(int, double);
        p_fn = &fn1;
        p_fn(1, 2.3);
        (****p_fn)(1, 2.3);

        std::cout << "\ntest pointer to fn[3]" << std::endl;
        // ptr to array fn[]
        // тип (*имя_указателя[size]) (параметры);
        int (*p_fnarr[3])(int, double) = {fn1, fn2, fn3};
        p_fnarr[0](1, 1.1);
        p_fnarr[1](2, 2.2);
        p_fnarr[2](3, 3.3);

        // ptr to class member fn
        // retval (ClassName::*PtrName) (Parmeters);
        //int (A::*p_A_fn)(int) = A::fn;  - ERROR
        int (A::*p_A_fn)(int) = &A::fn;
        A a;
        (a.*p_A_fn)(1);
        (a.*p_A_fn)(1);
    }

    A a();
    A a(int);

    void test_declaration_fn() {
        std::cout << "\ntest declaration fn" << std::endl;
        a();
        a(1);
    }

    A a() {
        std::cout << "call A a()" << std::endl;
        return A{};
    }
    A a(int) {
        std::cout << "call A a(int)" << std::endl;
        return A{};
    }

    int* f(int) {
        std::cout << "f()" << std::endl;
        return (int*)nullptr;
    }

    void test_declaration_lambda() {
        std::cout << "\ntest make lambda" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        auto make_lambda = [](int param) { return [param](int value) { return value + param; }; };
        auto l1          = make_lambda(1);
        auto l2          = make_lambda(2);
        std::cout << l1(1) << std::endl;
        std::cout << l2(1) << std::endl;

        std::cout << "\ntest 2 internal state lambda" << std::endl;
        int i = 0;
        // mutable разрешает менять внутриние переменные переданые в кв скобках как копии
        auto l11 = [i]() mutable { std::cout << ++i << std::endl; };
        l11();
        l11();
        std::cout << "i = " << i << std::endl;
        auto l12 = [&i]() { std::cout << ++i << std::endl; };
        l12();
        l12();
        std::cout << "i = " << i << std::endl;

        std::cout << "\ntest 3" << std::endl;
        // объявляется как
        // int*(int)
        // а кастица как
        // int* (*)(int)
        std::function<int*(int)> l3 = f;
        l3(1);
        int* (*pl3)(int) = *l3.target<int* (*)(int)>();
        if (!pl3)
            std::cout << "pl3 == nullptr" << std::endl;
        else
            (*pl3)(0);

        std::cout << "\ntest 4" << std::endl;
    }

    template <typename T>
    std::string fi0(T x) {
        return type_utils::type2name<T>();
    }

    template <typename T>
    std::string fi1(T& x) {
        return type_utils::type2name<T>();
    }

    template <typename T>
    std::string fi2(const T& x) {
        return type_utils::type2name<T>();
    }

    template <typename T>
    std::string fi3(T* x) {
        return type_utils::type2name<T>();
    }

    template <typename T>
    std::string fi4(const T* x) {
        return type_utils::type2name<T>();
    }

    template <typename T>
    std::string fi5(T&& x) {
        return type_utils::type2name<T>();
    }

    void f_int(int) {}

    void test_type_inference() {
        std::cout << "\ntest type inference" << std::endl;
        int i          = 1;
        const int ci   = 1;
        const int& cis = ci;
        int* pi        = &i;
        const int* cpi = &i;
        int m10[10];

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
        //arg - int,        param type - int,  T: int
        //arg - const int,  param type - int,  T: int
        //arg - const int&, param type - int,  T: int
        //arg - int*,       param type - int*, T: int*
        //arg - const int*, param type - int*, T: const int*
        //arg - void(int),  param type - void(*)(int), T: void(*)(int)
        //arg - int[10],  param type -               , T: int*

        std::cout << "\ntest 1 f(T& param)" << std::endl;
        // в параметр перетаскивается базовый тип& и константность если есть
        // Т - базовый тип и константность если есть в аргументе
        std::cout << "arg - int,        param type - int&,       T: " << fi1(i) << std::endl;
        std::cout << "arg - const int,  param type - const int&, T: " << fi1(ci) << std::endl;
        std::cout << "arg - const int&, param type - const int&, T: " << fi1(cis) << std::endl;
        std::cout << "arg - int*,       param type - int*&,      T: " << fi1(pi) << std::endl;
        std::cout << "arg - const int*, param type - const int*&,T: " << fi1(cpi) << std::endl;
        // для функции:
        std::cout << "arg - void(int),  param type - void(&)(int), T: " << fi1(f_int) << std::endl;
        // массив
        std::cout << "arg - int[10],  param type -               , T: " << fi1(m10) << std::endl;
        // вывод:
        //arg - int,        param type - int&,        T: int
        //arg - const int,  param type - const int&,  T: const int
        //arg - const int&, param type - const int&,  T: const int
        //arg - int*,       param type - int*&,       T: int*
        //arg - const int*, param type - const int*&, T: const int*
        //arg - void(int),  param type - void(&)(int), T: void(int)
        //arg - int[10],  param type - int(&)[10]    , T: int[10]

        std::cout << "\ntest 2 f(const T& param)" << std::endl;
        // в параметр перетаскивается базовый тип& + добавляется всегда константность
        // Т всегда базовый тип
        std::cout << "arg - int,        param type - const int&, T: " << fi2(i) << std::endl;
        std::cout << "arg - const int,  param type - const int&, T: " << fi2(ci) << std::endl;
        std::cout << "arg - const int&, param type - const int&, T: " << fi2(cis) << std::endl;
        // вывод:
        //arg - int,        param type - const int&, T: int
        //arg - const int,  param type - const int&, T: int
        //arg - const int&, param type - const int&, T: int

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
        //arg - int*,        param type - int*,       T: int
        //arg - const int*,  param type - const int*, T: const int
        //arg - void(int),   param type - void(*)(int), T: void(int)
        //arg - int[10],     param type - int*,         T: int

        std::cout << "\ntest 4 f(const T* param)" << std::endl;
        // в параметр перетаскивается базовый тип* + добавляется всегда константность
        // Т всегда базовый тип
        std::cout << "arg - int*,        param type - int*,       T: " << fi4(pi) << std::endl;
        std::cout << "arg - const int*,  param type - const int*, T: " << fi4(cpi) << std::endl;
        // вывод:
        //arg - int*,        param type - int*,       T: int
        //arg - const int*,  param type - const int*, T: int

        std::cout << "\ntest 5 f(T&& param)" << std::endl;
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
        std::cout << "arg - int rvalue,  param type - int&&,       T: " << fi5(123) << std::endl;
        std::cout << "arg - int*,        param type - int*&,       T: " << fi5(pi) << std::endl;
        std::cout << "arg - const int*,  param type - const int*&, T: " << fi5(cpi) << std::endl;
        // для функции:
        std::cout << "arg - void(int),  param type -             , T: " << fi5(f_int) << std::endl;
        // массив
        std::cout << "arg - int[10],     param type -    ,         T: " << fi5(m10) << std::endl;
        //вывод:
        //arg - int,         param type - int&,        T: int&
        //arg - const int,   param type - const int&,  T: const int&
        //arg - const int&,  param type - const int&,  T: const int&
        //arg - int rvalue,  param type - int&&,       T: int
        //arg - int*,        param type - int*&,       T: int*&
        //arg - const int*,  param type - const int*&, T: const int*&
        //arg - void(int),  param type -             , T: void(&)(int)
        //arg - int[10],     param type -    ,         T: m10_t& (typedef int m10_t[10];)

        // typeid() пока выводит бред, должен показать что это const int&, а показывает int
        std::cout << ">" << typeid(cis).name() << std::endl;
    }

    template <typename C>
    auto f0_ta(C&& c) {
        return (c[0]);
    }

    template <typename C>
    decltype(auto) f1_ta(C&& c) {
        return c[0];
    }

    template <typename C>
    auto f2_ta(C&& c) {
        return std::forward<C>(c)[0];
    }

    template <typename C>
    auto f3_ta(C&& c) -> decltype(std::forward<C>(c)[0]) {
        return std::forward<C>(c)[0];
    }

    template <typename C>
    decltype(auto) f4_ta(C&& c) {
        return std::forward<C>(c)[0];
    }

    void test_auto() {
        std::cout << "\ntest auto" << std::endl;

        // чтобы переменная auto была в точности такого же типа как ее инициализатор то надо объявлять ее:
        // decltype(auto) var = ...
        // чтобы функция(auto f()) возвращала ссылку есть 2 варианта:
        // 1 спецификатор возвращаемого значения:
        // auto f() -> ... { ... }
        // 2 объявить функцию  decltype(auto) f() { return reference; } и возвратить тип ссылки
        // PS: для универсальных ссылок пользовать std::forward<>()

        std::vector<int> v{1, 2, 3};
        std::cout << "> " << type_utils::type2name<decltype(v[0])>() << std::endl;

        auto printv = [&] {for(const auto& i: v) std::cout << i << " "; std::cout<<std::endl; };

        std::cout << "\ntest 0" << std::endl;
        decltype(auto) a0 = f0_ta(v);
        a0                = 10;
        std::cout << "a0 type : " << type_utils::type2name<decltype(a0)>() << std::endl;
        printv();

        decltype(auto) a1 = f1_ta(v);
        a1                = 11;
        std::cout << "a1 type : " << type_utils::type2name<decltype(a1)>() << std::endl;
        printv();

        decltype(auto) a2 = f2_ta(v);
        a2                = 12;
        std::cout << "a2 type : " << type_utils::type2name<decltype(a2)>() << std::endl;
        printv();

        decltype(auto) a3 = f3_ta(v);
        a3                = 13;
        std::cout << "a3 type : " << type_utils::type2name<decltype(a3)>() << std::endl;
        printv();

        decltype(auto) a4 = f3_ta(v);
        a4                = 14;
        std::cout << "a4 type : " << type_utils::type2name<decltype(a4)>() << std::endl;
        printv();
    }
}