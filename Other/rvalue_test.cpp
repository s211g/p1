
#include "rvalue_test.hpp"
#include "TypeUtils.hpp"

namespace rvalue_test {

    int&& f() {
        static int i{0};
        return std::move(i);
    }

    template <typename T>
    void print_type(T&& t) {
        std::cout << "type T             : " << type_utils::type2name<T>() << std::endl;
        std::cout << "type t             : " << type_utils::type2name<decltype(t)>() << std::endl;
        std::cout << "forward return type: " << type_utils::type2name<decltype(std::forward<T>(t))>() << std::endl;
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


        //  void print_type(T&& t) { std::forward<T>(t) }
        // если передавать временные значения или неим ссылки на rvalue, то forward возвращает(кастит) неим ссылки rvalue
        print_type(5);
        print_type(int(1));
        print_type(std::move(srv4));
        // если передать lvalue или ссылку(& или &&)
        int j{0};
        int& ref_j   = j;
        int&& rref_j = std::move(j);
        print_type(j);
        print_type(ref_j);
        print_type(rref_j);
    }
}