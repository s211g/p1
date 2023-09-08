
#include "rvalue_test.hpp"
#include "TypeUtils.hpp"

namespace rvalue_test {

    int&& f() {
        static int i{0};
        return std::move(i);
    }

    template <typename T>
    void print_type(const char* caption, T&& t) {
        std::cout << caption << ", type T             : " << type_utils::type2name<T>() << std::endl;
        std::cout << caption << ", type t             : " << type_utils::type2name<decltype(t)>() << std::endl;
        std::cout << caption << ", forward return type: " << type_utils::type2name<decltype(std::forward<T>(t))>() << std::endl;
    }

    void test1() {
        std::cout << "test1" << std::endl;

        int i{0};
        // std::move
        // template <class _Ty>
        // _NODISCARD constexpr remove_reference_t<_Ty>&& move(_Ty&& _Arg) noexcept { // forward _Arg as movable
        // return static_cast<remove_reference_t<_Ty>&&>(_Arg);
        // }

        // возвращает неименованную ссылку на rvalue
        int&& srv1 = static_cast<int&&>(i);
        srv1       = 3;
        std::cout << "i = " << i << std::endl;
        std::cout << "srv1 = " << srv1 << std::endl;

        int&& srv2 = f();
        std::cout << "srv2 = " << srv2 << std::endl;

        // нельзя инициализировать другой ссылкой rvalue
        // int&& srv3 = srv1;

        /*
        template <class _Ty>
        _NODISCARD constexpr _Ty&& forward(
            remove_reference_t<_Ty>& _Arg) noexcept { // forward an lvalue as either an lvalue or an rvalue
            return static_cast<_Ty&&>(_Arg);
        }

        template <class _Ty>
        _NODISCARD constexpr _Ty&& forward(remove_reference_t<_Ty>&& _Arg) noexcept { // forward an rvalue as an rvalue
            static_assert(!is_lvalue_reference_v<_Ty>, "bad forward call");
            return static_cast<_Ty&&>(_Arg);
        }
        */

        int&& srv4 = std::forward<int>(srv1);
        std::cout << "srv4 = " << srv4 << std::endl;


        // void print_type(T&& t) { std::forward<T>(t) }
        // если передавать временные значения или неим ссылки на rvalue, то forward возвращает(кастит) неим ссылки rvalue
        print_type("5", 5);
        print_type("int(1)", int(1));
        print_type("std::move(srv4)", std::move(srv4));
        // вывод:
        // 5, type T             : int
        // 5, type t             : int&&
        // 5, forward return type: int&&
        // int(1), type T             : int
        // int(1), type t             : int&&
        // int(1), forward return type: int&&
        // std::move(srv4), type T             : int
        // std::move(srv4), type t             : int&&
        // std::move(srv4), forward return type: int&&

        // если передать lvalue или ссылку(& или &&)
        int j{0};
        int& ref_j   = j;
        int&& rref_j = std::move(j);
        print_type("j", j);
        print_type("ref_j", ref_j);
        print_type("rref_j", rref_j);
        // вывод:
        // j, type T             : int&
        // j, type t             : int&
        // j, forward return type: int&
        // ref_j, type T             : int&
        // ref_j, type t             : int&
        // ref_j, forward return type: int&
        // rref_j, type T             : int&
        // rref_j, type t             : int&
        // rref_j, forward return type: int&

        // правила свертки ссылок
        // && & = &
        // && && = &&
        using T1 = int&;
        using T2 = T1&&;
        std::cout << "type T1 : " << type_utils::type2name<T1>() << std::endl;
        std::cout << "type T2 : " << type_utils::type2name<T2>() << std::endl;
        using T3 = int&&;
        using T4 = T3&;
        using T5 = T3&&;
        std::cout << "type T3 : " << type_utils::type2name<T3>() << std::endl;
        std::cout << "type T4 : " << type_utils::type2name<T4>() << std::endl;
        std::cout << "type T5 : " << type_utils::type2name<T5>() << std::endl;
        // вывод:
        // type T1 : int&
        // type T2 : int&
        // type T3 : int&&
        // type T4 : int&
        // type T5 : int&&
    }

    void test2() {
        std::cout << "test2" << std::endl;

        // вывод типа универсальной ссылки тот же что
        // правила свертки ссылок
        // && & = &
        // && && = &&

        int i{0};
        int& refi = i;

        // как ссылка на r-value
        auto&& ref1 = 5;
        auto&& ref2 = int();
        auto&& ref3 = std::move(i);

        // как ссылка
        auto&& ref4 = i;
        auto&& ref5 = ref1;
        auto&& ref6 = refi;

        std::cout << "ref1 type :" << type_utils::type2name<decltype(ref1)>() << std::endl;
        std::cout << "ref2 type :" << type_utils::type2name<decltype(ref2)>() << std::endl;
        std::cout << "ref3 type :" << type_utils::type2name<decltype(ref3)>() << std::endl;
        std::cout << "ref4 type :" << type_utils::type2name<decltype(ref4)>() << std::endl;
        std::cout << "ref5 type :" << type_utils::type2name<decltype(ref5)>() << std::endl;
        std::cout << "ref6 type :" << type_utils::type2name<decltype(ref6)>() << std::endl;

        // Person&& rValueRef  = MakePerson(); // rValueRef --- rvalue ссылка
        // auto&& universalRef = rValueRef;    // Не rvalue ссылка

        // Тип аргумента функции выводится не полностью.
        // template <typename T>
        // void DoSomething(std::vector<T> && rValueRef2); // Это rvalue ссылка.

        // Тип аргумента функции выводится полностью.
        // template <typename T>
        // void DoSomething(T && universalRef2); // Это не rvalue ссылка.

        // Операции по - умолчанию
        // Пусть класс переопределяет конструктор копирования и операцию копирующего присваивания.В этом случае,
        // компилятор не будет их создавать.Чтобы заставить  его создать стандартные перемещающие операции
        // надо определить их = default.
        // class SomeClass {
        // public:
        //     SomeClass(const SomeClass& other) { // Переопределение copy-конструктора
        //     }
        //     SomeClass& operator=(const SomeClass& other) { // Переопределение операции копирующего присваивания
        //     }
        //     SomeClass(SomeClass&&) = default;
        //     SomeClass& operator=(SomeClass&&) = default;
        // };
    }

    void test3() {
        std::cout << "test3" << std::endl;

        // Стандарт C89 определял lvalue как object locator, т.е. объект с идентифицируемым местом в памяти.
        // Соответственно, всё, что не подходило под это определение, входило в категорию rvalue.

        int i;
        auto fn1 = [](int& r) {};
        auto fn2 = [](int& r) -> int& { return r; };

        int&& rref = int(0);


        // справа ИМЕНОВАННАЯ r-value ссылка ведет себя как обычная ссылка
        int i     = rref;
        int& ref1 = rref; // можно ссылаться на ИМЕНОВАННУЮ ссылку r-value
        fn1(rref);        // также r-value ссылку можно передать в аргумент с сылкой

        // инициализация lvalue ссылок
        // не константные ссылки не могут ссылатьсяна временные и НЕИМЕНОВАННЫЕ ссылки r-value
        //int& ref2       = int(0);    // но нельзя ссылаться на временное значение
        //int& ref2 = std::move(rref); // нельзя ссылаться на НЕИМЕНОВАННУЮ ссылку r-value
        int& ref3 = fn2(i); // ссылаться на неименованные обычные ссылки можно

        // обычные константные ссылки могут ссылаться на что угодно
        const int& ref3 = int(0);          // можно сделать константную ссылку на временное значение
        const int& ref4 = std::move(rref); // можно сделать константную ссылку на НЕИМЕНОВАННУЮ ссылку r-value
    }

}