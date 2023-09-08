
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
        //}

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
        //вывод:
        //5, type T             : int
        //5, type t             : int&&
        //5, forward return type: int&&
        //int(1), type T             : int
        //int(1), type t             : int&&
        //int(1), forward return type: int&&
        //std::move(srv4), type T             : int
        //std::move(srv4), type t             : int&&
        //std::move(srv4), forward return type: int&&

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
}