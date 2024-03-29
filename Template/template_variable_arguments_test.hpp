#pragma once

#include <string>
#include <iostream>
#include <ostream>


namespace template_variable_arguments_test {

    void test_sum17();

    template <typename... T>
    auto sum17(const T&... x) {
        return (x + ...);
    }

    template <typename... T>
    auto sum_pow(const T&... x) {
        return ((x * x) + ...);
    }

    void test_sum14();

    template <typename T1>
    auto sum(const T1& x1) {
        std::cout << "sum(const T1& x1) x1 = " << x1 << std::endl;
        return x1;
    }

    template <typename T1, typename... T>
    auto sum(const T1& x1, const T&... x) {
        std::cout << "sum(const T1& x1, const T&... x) x1 = " << x1 << std::endl;
        return x1 + sum(x...);
    }

    void test_sum14_constexpr();

    template <size_t...>
    constexpr size_t sum14_constexpr = 0;

    template <size_t first, size_t... other>
    constexpr size_t sum14_constexpr<first, other...> = first + sum14_constexpr<other...>;

    template <typename... types>
    constexpr size_t sum14_constexpr_sizeof = sum14_constexpr<sizeof(types)...>;

    void test_print();

    inline void print() { std::cout << "print() " << std::endl; }                          // Нечего печатать.
    inline void print(std::ostream&) { std::cout << "print(std::ostream&)" << std::endl; } // Нечего печатать.

    template <class First, class... Other>
    void print(std::ostream& os,
               const First& first, const Other&... other) {
        os << first;
        print(os, other...);
    }

    template <class First, class... Other>
    void print(const First& first, const Other&... other) {
        std::cout << first;
        print(other...);
    }

    template <class... Params>
    void print_array(const Params&... params) {
        std::string param[]{std::to_string(params)...};
        for (auto& v : param)
            std::cout << v << ",";
        std::cout << std::endl;
    }

    void test_tgroup();

    template <typename... T>
    struct Group;

    template <typename T1>
    struct Group<T1> {
        T1 t1_;
        Group() = default;
        explicit Group(const T1& t1) :
            t1_(t1) {
            std::cout << "Group<T1>::Group(const T1& t1) t1 = " << t1_ << std::endl;
        }
        explicit Group(T1&& t1) :
            t1_(std::move(t1)) {
            std::cout << "Group<T1>::Group(const T1&& t1) t1 = " << t1_ << std::endl;
        }
        explicit operator const T1&() const {
            std::cout << "Group<T1>::operator const T1&() t1 = " << t1_ << std::endl;
            return t1_;
        }
        explicit operator T1&() {
            std::cout << "Group<T1>::operator T1&() t1 = " << t1_ << std::endl;
            return t1_;
        }
    };

    template <typename T1, typename... T>
    struct Group<T1, T...> : Group<T...> {
        T1 t1_;
        Group() = default;
        explicit Group(const T1& t1, T&&... t) :
            Group<T...>(std::forward<T>(t)...), t1_(t1) {
            std::cout << "Group<T1, T...>::Group(const T1& t1,  T&&... t) t1 = " << t1_ << std::endl;
        }
        explicit Group(T1&& t1, T&&... t) :
            Group<T...>(std::forward<T>(t)...), t1_(std::move(t1)) {
            std::cout << "Group<T1, T...>::Group(const T1&& t1,  T&&... t) t1 = " << t1_ << std::endl;
        }
        explicit operator const T1&() const {
            std::cout << "Group<T1, T...>::operator const T1&() t1 = " << t1_ << std::endl;
            return t1_;
        }
        explicit operator T1&() {
            std::cout << "Group<T1, T...>::operator T1&() t1 = " << t1_ << std::endl;
            return t1_;
        }
    };

    template <typename... T>
    auto makeGroup(T&&... t) {
        return Group<T...>(std::forward<T>(t)...);
    }

    void test_overload_set();

    template <typename... F>
    struct overload_set;

    template <typename F1>
    struct overload_set<F1> : public F1 {
        overload_set(F1&& f1) :
            F1(std::move(f1)) {}
        overload_set(const F1& f1) :
            F1(f1) {}
        using F1::operator();
    };

    template <typename F1, typename... F>
    struct overload_set<F1, F...> : public F1, public overload_set<F...> {
        overload_set(F1&& f1, F&&... f) :
            F1(std::move(f1)), overload_set<F...>(std::forward<F>(f)...) {}
        overload_set(const F1& f1, F&&... f) :
            F1(f1), overload_set<F...>(std::forward<F>(f)...) {}
        using F1::operator();
    };

    template <typename... F>
    auto overload(F&&... f) {
        return overload_set<F...>(std::forward<F>(f)...);
    }

    void test_overload_set17();

    template <typename... F>
    struct overload_set17 : public F... {
        overload_set17(F&&... f) :
            F(std::forward<F>(f))... {}
        using F::operator()...;
    };

    template <typename... F>
    auto overload17(F&&... f) {
        return overload_set17<F...>(std::forward<F>(f)...);
    }

    void test_forward();

    class A {
    public:
        int i;

        /*
        A(int i_) :
            i(i_) {
            std::cout << "A(int i)" << std::endl;
        }
        */

        A(int& i_) :
            i(i_) {
            std::cout << "A(int& i)" << std::endl;
        }

        A(const int& i_) :
            i(i_) {
            std::cout << "A(const int& i)" << std::endl;
        }

        A(int&& i_) :
            i(i_) {
            std::cout << "A(int&& i)" << std::endl;
        }

        A() {
            std::cout << "A()" << std::endl;
        }

        ~A() = default;

        A(const A& t) {
            std::cout << "A(const T& t)" << std::endl;
        }

        A(A& t) {
            std::cout << "A(T& t)" << std::endl;
        }

        A(A&& t) {
            std::cout << "A(T&& t)" << std::endl;
        }

        A& operator=(A& a) {
            std::cout << "A::operator(T& t)" << std::endl;
            return *this;
        }

        A& operator=(const A& a) {
            std::cout << "A::operator(T& t)" << std::endl;
            return *this;
        }

        void print() const {
            std::cout << "this[" << this << "] i = " << i << std::endl;
        }
    };

    template <typename T>
    void f(T&& t) {
        T t1 = std::forward<T>(t);
        t1.print();
    }

    template <typename T>
    void f2(T&& t) {
        T t1(std::forward<T>(t));
        t1.print();
    }

    template <typename T>
    void f4(T&& t) {
        A a(std::forward<T>(t));
    }

    template <typename T>
    bool less(T&& t1, T&& t2) {
        return t1 < t2;
    }

    inline bool less2(int&& t1, int&& t2) {
        return t1 < t2;
    }

    template <typename T>
    bool less3(T&& t1, T&& t2) {
        return t1 < t2;
    }


    void test_count_types();

    template <typename... T>
    auto print_array_sizes(const T&... x) {
        std::string s = ((std::to_string(sizeof(x)) + std::string(",")) + ...);
        if (!s.empty())
            s.resize(s.length() - 1);
        std::cout << " sizes: " << s << std::endl;
    }

    //    template <class... Types>
    //    constexpr size_t t_count = 0;

    template <class... Types>
    constexpr size_t t_count = 0;

    template <class T, class... Types>
    constexpr size_t t_count<T, T, Types...> = 1 + t_count<T, Types...>;

    template <class T, class X, class... Types>
    constexpr size_t t_count<T, X, Types...> = t_count<T, Types...>;

    template <typename... T>
    struct CT;


    template <typename T>
    struct CT<T> {
        int value;
        CT() {
            value = 0;
        }
    };

    template <typename T, typename... Other>
    struct CT<T, T, Other...> {
        int value;
        CT() {
            value = 1 + CT<T, Other...>().value;
        }
    };

    template <typename T, typename X, typename... Other>
    struct CT<T, X, Other...> {
        int value;
        CT() {
            value = CT<T, Other...>().value;
        }
    };


    template <class... Other>
    int count_types(const Other&... t) {
        std::cout << "count_types(const Other&... t)" << std::endl;
        return 0;
    }

    template <class T, class... Other>
    int count_types(const T& t, const T& t1, const Other&... types);
    template <class T, class X, class... Other>
    int count_types(const T& t, const X& t1, const Other&... types);


    template <class T, class... Other>
    int count_types(const T& t, const T& t1, const Other&... types) {
        std::cout << "count_types(T& t, T& t1, Other&... types) return "
                  << "t = " << t << " t1 = " << t1 << std::endl;
        print_array_sizes(t, t1, types...);

        int ret = 1 + count_types(t, types...);

        //    std::cout << "count_types(T& t, T& t1, Other&... types) return " << ret << std::endl;
        //    std::cout << "t = " << t << " t1 = " << t1 << std::endl;
        //    std::cout << "sizeof(t) = " << sizeof(t) << " sizeof(t1) = " << sizeof(t1) << std::endl;
        return ret;
    }

    template <class T, class X, class... Other>
    int count_types(const T& t, const X& t1, const Other&... types) {
        std::cout << "count_types(T& t, X& t1, Other&... types) return "
                  << "t = " << t << " t1 = " << t1 << std::endl;
        print_array_sizes(t, t1, types...);

        int ret = count_types(t, types...);

        //std::cout << "count_types(T& t, X& t1, Other&... types) return " << ret << std::endl;
        //std::cout << "t = " << t << " t1 = " << t1 << std::endl;
        //std::cout << "sizeof(t) = " << sizeof(t) << " sizeof(t1) = " << sizeof(t1) << std::endl;
        return ret;
    }

    void test_type_by_index();


    template <class... Types>
    struct Type_list {
    };

    template <size_t i, class L>
    struct t_at {};

    template <size_t i, class T, class... Tail>
    struct t_at<i, Type_list<T, Tail...>> {
        using type =
            typename t_at<i - 1, Type_list<Tail...>>::type;
    };

    template <class T, class... Tail>
    struct t_at<0, Type_list<T, Tail...>> {
        using type = T;
    };


    void test_fold_expressions();

    //template <class... Types>
    // Узнать размер пакета можно с помощью sizeof…(имя пакета) - tl<int, char, bool>::size  = 3
    // sizeof(Types)... раскрывается в sizeof(int), sizeof(char), sizeof(bool)

    template <class... Types>
    struct tl {
        static constexpr auto size = sizeof...(Types);
    };

    //(pack op ...) 	Унарная правоассоциативная свертка
    template <typename... Args>
    auto sum_un_r_fold(Args... args) {
        return (("a" + args + "b") + ...);
        //return args + ...);
    }

    template <typename... Args>
    void print_un_r_fold(Args... args) {
        ((std::cout << args), ...);
    }

    template <typename... Args>
    void print_sizeof_args(Args... args) {
        print_un_r_fold(sizeof(args)...);
    }

    //(… op pack) 	Унарная левоассоциативная свертка
    template <typename... Args>
    auto sum_un_l_fold(Args... args) {
        return (... + args);
    }

    //(pack op… op init) 	Бинарная правоассоциативная свертка
    // sum_bin_r_fold_17(f,1,2,3) = 1 + (2 + (3 + f)) = 123f
    template <typename Arg, typename... Args>
    auto sum_bin_r_fold_17(Arg first, Args... args) {
        return (args + ... + first);
    }

    template <typename Arg>
    auto sum_bin_r_fold_14(Arg arg) {
        return arg;
    }

    // f + ( 1 + (2 + (3)))
    template <typename... Args, typename Arg>
    auto sum_bin_r_fold_14(Arg init, Args... rest) {
        return init + sum_bin_r_fold_14(rest...);
    }

    //(init op… op pack) 	Бинарная левоассоциативная свертка
    // sum_bin_r_fold_17(f,1,2,3) = ((f + 1) + 2) + 3 = f123
    template <typename Arg, typename... Args>
    auto sum_bin_l_fold_17(Arg first, Args... args) {
        return (first + ... + args);
    }


    inline void
    test() {
        std::cout << "variable arguments of template tests" << std::endl;

        // test_sum17();
        //test_sum14();
        //test_sum14_constexpr();
        //test_print();
        //test_tgroup();
        //test_overload_set();
        //test_overload_set();
        //test_overload_set17();
        test_forward();
        //test_count_types();
        //test_type_by_index();
        //test_fold_expressions();
    }
}