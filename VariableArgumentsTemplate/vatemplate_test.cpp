#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <functional>
#include <memory>

#include "vatemplate_test.hpp"

namespace vatemplate_test {

    void test_sum17() {
        std::cout << "test sum c++17" << std::endl;
        std::cout << "= " << sum17(1, 3, 5) << std::endl;
        std::cout << "sum_pow(1, 3) = " << sum_pow(1, 3) << std::endl;
    }

    void test_sum14() {
        std::cout << "test sum c++14" << std::endl;
        auto s = sum(1, 3, 5);
        std::cout << "= " << s << std::endl;
    }

    void test_sum14_constexpr() {
        std::cout << "test sum c++14  constexpr" << std::endl;

        auto s = sum14_constexpr<1, 3, 5>;
        std::cout << "= " << s << std::endl;

        std::cout << "sum14_constexpr_sizeof<int, char> = " << sum14_constexpr_sizeof<int, char> << std::endl;
    }

    void test_print() {
        std::cout << "test print" << std::endl;

        std::cout << "print_array(1, 2, 3) :" << std::endl;
        print_array(1, 2, 3);

        print(std::cout, "1", "2");
        print("2", "3");
    }

    void test_tgroup() {
        std::cout << "test types group" << std::endl;

        int ii  = 11;
        long ll = 22;
        Group<int, long> g0(ii, std::move(ll));
        std::cout << std::endl;

        auto g00 = makeGroup(10, 2.2, std::string("cde"));
        std::cout << std::endl;

        Group<int, long, std::string> g1(1, 2, "abc");
        std::cout << std::endl;

        std::string s = static_cast<std::string>(g1);
        int i         = static_cast<int>(g1);
        long l        = static_cast<long>(g1);
        std::cout << "s: " << s << " int: " << i << " long: " << l << std::endl
                  << std::endl;

        const std::string& s1 = static_cast<const std::string&>(g1);
        const int& i1         = static_cast<const int&>(g1);
        const long& l1        = static_cast<const long&>(g1);
        std::cout << "s: " << s1 << " int: " << i1 << " long: " << l1 << std::endl
                  << std::endl;

        const std::string& s2(g1);
        const int& i2(g1);
        const long& l2(g1);
        std::cout << "s: " << s2 << " int: " << i2 << " long: " << l2 << std::endl
                  << std::endl;
    }

    void test_overload_set() {
        typedef int intt;
        std::cout << "test overload set" << std::endl;
        auto os = overload(

            [](int* i) { std::cout << "int* " << *i << std::endl; },
            [](long* i) { std::cout << "long* " << *i << std::endl; },
            [](long long* i) { std::cout << "long long* " << *i << std::endl; },
            [](float* i) { std::cout << "float* " << *i << std::endl; },
            [](double* i) { std::cout << "double* " << *i << std::endl; },
            //[](intt* i) { std::cout << "intt* " << *i << std::endl; },

            [](double i) { std::cout << "double " << i << std::endl; },
            [](float i) { std::cout << "float " << i << std::endl; },
            //[](intt i) { std::cout << "intt " << i << std::endl; },
            [](int i) { std::cout << "int " << i << std::endl; },
            [](long i) { std::cout << "long " << i << std::endl; },
            [](long long i) { std::cout << "long long " << i << std::endl; }

        );

        os(10);
        os((long long)11);

        float f      = 1.2;
        double d     = 2.3;
        int i        = 11;
        long l       = 12;
        long long ll = 22;
        intt ii      = 33;

        std::cout << std::endl;
        os(f);
        os(d);
        os(i);
        os(ii);
        os(l);
        os(ll);

        std::cout << std::endl;
        os(&f);
        os(&d);
        os(&i);
        os(&ii);
        os(&l);
        os(&ll);
    }

    void test_overload_set17() {
        typedef int intt;
        std::cout << "test overload set17" << std::endl;
        auto os = overload17(

            [](int* i) { std::cout << "int* " << *i << std::endl; },
            [](long* i) { std::cout << "long* " << *i << std::endl; },
            [](long long* i) { std::cout << "long long* " << *i << std::endl; },
            [](float* i) { std::cout << "float* " << *i << std::endl; },
            [](double* i) { std::cout << "double* " << *i << std::endl; },
            //[](intt* i) { std::cout << "intt* " << *i << std::endl; },

            [](double i) { std::cout << "double " << i << std::endl; },
            [](float i) { std::cout << "float " << i << std::endl; },
            //[](intt i) { std::cout << "intt " << i << std::endl; },
            [](int i) { std::cout << "int " << i << std::endl; },
            [](long i) { std::cout << "long " << i << std::endl; },
            [](long long i) { std::cout << "long long " << i << std::endl; }


        );

        os(10);
        os((long long)11);

        float f      = 1.2;
        double d     = 2.3;
        int i        = 3;
        intt ii      = 4;
        long l       = 5;
        long long ll = 6;

        std::cout << std::endl;
        os(f);
        os(d);
        os(i);
        os(ii);
        os(l);
        os(ll);

        std::cout << std::endl;
        os(&f);
        os(&d);
        os(&i);
        os(&ii);
        os(&l);
        os(&ll);
    }

    void test_forward() {
        std::cout << "test forward" << std::endl;

        int i = 0;
        A a(i);
        A& ra        = a;
        const A& cra = a;

        std::cout << "A(forward) ----------------------" << std::endl;

        std::cout << "1" << std::endl;
        A a0(std::forward<A>(std::move(a)));
        std::cout << "2" << std::endl;
        A a00(std::forward<A>(a));
        std::cout << "3" << std::endl;
        A a000(std::forward<A>(ra));

        std::cout << "A() = forward ----------------------" << std::endl;

        std::cout << "1" << std::endl;
        A a21 = std::forward<A>(a);
        std::cout << "2" << std::endl;
        A a22 = std::forward<A>(ra);
        std::cout << "3" << std::endl;
        A a23 = std::forward<A>(std::move(a));

        std::cout << "a = forward ----------------------" << std::endl;

        std::cout << "1" << std::endl;
        a23 = std::forward<A>(std::move(a));

        std::cout << "A(...) ----------------------" << std::endl;

        std::cout << "1" << std::endl;
        A a1(a);
        std::cout << "2" << std::endl;
        A a2(ra);
        std::cout << "3" << std::endl;
        A a3(std::move(a));
        std::cout << "4" << std::endl;
        A a4 = a;
        std::cout << "5" << std::endl;
        A a5 = std::move(a);


        std::cout << "f(...) ----------------------" << std::endl;

        std::cout << "1" << std::endl;
        f(a);
        std::cout << "2" << std::endl;
        f(ra);
        std::cout << "3" << std::endl;
        f(A());
        std::cout << "4" << std::endl;
        f(std::move(a));

        std::cout << "f2(...) ----------------------" << std::endl;

        std::cout << "1" << std::endl;
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! call f2< A& >(A&)
        a.print();
        f2(a);
        std::cout << "2" << std::endl;
        f2(ra);
        std::cout << "3" << std::endl;
        f2(cra);
        std::cout << "4" << std::endl;
        f2(A());
        std::cout << "5" << std::endl;
        f2(std::move(a));
        std::cout << "6" << std::endl;
        auto f3 = [](A& a) {
            f2(a);
        };
        f3(a);

        std::cout << "f2(int) ----------------------" << std::endl;
        std::cout << "1" << std::endl;
        f4(i);
        std::cout << "2" << std::endl;
        f4(std::move(i));
        std::cout << "3" << std::endl;
        f4(a);
        std::cout << "4" << std::endl;
        f4(cra);
        std::cout << "5" << std::endl;
        f4(std::move(a));

        std::cout << "less ----------------------" << std::endl;
        int v1 = 11;
        int v2 = 12;
        std::cout << ">" << less(v1, v2) << std::endl;
        std::cout << ">" << less(std::move(v1), std::move(v2)) << std::endl;
        std::cout << ">" << less3(v1, v2) << std::endl;
        std::cout << ">" << less3(std::move(v1), std::move(v2)) << std::endl;
    }


    void test_count_types() {
        std::cout << "test count types" << std::endl;

        int i  = 0;
        char l = 0;
        //print_array_sizes(i, l, l, i);

        std::cout << "count_types = " << count_types(i, l, l, l, i, i, l, i, i, i) << std::endl;
        std::cout << "count_types = " << count_types(int(1), int(2), double(3), int(4), double(5)) << std::endl;

        std::cout << "---------------" << std::endl;
        std::cout << "count_types = " << count_types(int(1), double(2), int(3), int(4), long(5), int(6)) << std::endl;

        std::cout << "---------------" << std::endl;
        std::cout << "count_types = " << count_types(int(1), int(2), double(3), int(4), long(5), int(6)) << std::endl;


        std::cout << "---------------" << std::endl;
        std::cout << "t_count = " << t_count<int, double, int> << std::endl;

        std::cout << "---------------" << std::endl;
        std::cout << "t_count = " << t_count<int, int, double> << std::endl;


        std::cout << "---------------" << std::endl;
        std::cout << "CT = " << CT<int, double, int, int, double, int>().value << std::endl;

        std::cout << "---------------" << std::endl;
        std::cout << "CT = " << CT<int, int, double, int, double, int>().value << std::endl;
    }

    void test_type_by_index() {
        std::cout << "test type by index" << std::endl;

        std::cout << "0>" << sizeof(t_at<0, Type_list<bool, int, double>>::type) << std::endl;
        std::cout << "1>" << sizeof(t_at<1, Type_list<bool, int, double>>::type) << std::endl;
        std::cout << "2>" << sizeof(t_at<2, Type_list<bool, int, double>>::type) << std::endl;

        //template <size_t i, class L>
        //using t_at_t = typename t_at<i, L>::type;
        // Теперь, например,
        //   t_at_t<2, Type_list<bool, char, int, float>>
        // равно int.
    }

    void test_fold_expressions() {
        std::cout << "test_fold_expressions" << std::endl;

        //(pack op ...) 	Унарная правоассоциативная свертка
        std::cout << "sum_un_r_fold(1,2,3) = " << sum_un_r_fold(std::to_string(1), std::to_string(2), std::to_string(3)) << std::endl;
        std::cout << "print_un_r_fold(1,2,3) = ";
        print_un_r_fold(1, 2, 3);
        std::cout << std::endl;

        //(… op pack) 	Унарная левоассоциативная свертка
        std::cout << "sum_un_l_fold(1,2,3) = " << sum_un_l_fold(std::to_string(1), std::to_string(2), std::to_string(3)) << std::endl;

        //(pack op… op init) 	Бинарная правоассоциативная свертка
        std::cout << "sum_bin_r_fold_17(f,1,2,3) = " << sum_bin_r_fold_17(std::string("f"), std::to_string(1), std::to_string(2), std::to_string(3)) << std::endl;
        std::cout << "sum_bin_r_fold_14(f,1,2,3) = " << sum_bin_r_fold_14(std::string("f"), std::to_string(1), std::to_string(2), std::to_string(3)) << std::endl;

        //(init op… op pack) 	Бинарная левоассоциативная свертка
        std::cout << "sum_bin_l_fold_17(f,1,2,3) = " << sum_bin_l_fold_17(std::string("f"), std::to_string(1), std::to_string(2), std::to_string(3)) << std::endl;
    }

}