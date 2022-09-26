#include <iostream>
#include <array>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <utility>
#include <tuple>

#include "containers_test.hpp"

namespace containers_test {

    void test_array() {
        std::cout << "\ntest std::array" << std::endl;

        std::array<int, 3> a3 = {{1, 2, 3}};
        std::for_each(a3.begin(), a3.end(), [](int& i) { ++i; });
        std::for_each(a3.begin(), a3.end(), [](int& i) { static int c=0; std::cout << "[" << c++<< "] : " << i << std::endl; });
    }

    class A {
    public:
        int v;
        A(int v1, int v2) :
            v(v1 + v2) { std::cout << "A::A(" << v1 << "," << v2 << ")" << std::endl; }
    };

    void test_vector() {
        std::cout << "\ntest std::vector" << std::endl;
        std::vector<A> va;
        //va.push_back(1, 2);  error
        va.emplace_back(1, 2);
        va.emplace_back(3, 4);
        std::for_each(va.begin(), va.end(), [](A& a) { static int c=0; std::cout << "[" << c++<< "] : a.v = " << a.v  << std::endl; });
    }

    void test_set() {
        std::cout << "\ntest std::set" << std::endl;
        std::set<int> si;
        si.insert(1);
        si.insert(2);
        si.insert(3);
        std::for_each(si.begin(), si.end(), [](const int& i) { static int c=0; std::cout << "[" << c++<< "] : " << i  << std::endl; });
        si.erase(2);
        std::for_each(si.begin(), si.end(), [](const int& i) { static int c=0; std::cout << "[" << c++<< "] : " << i  << std::endl; });
        auto it = si.find(1);
        if (it != si.end())
            std::cout << "find(1) = true" << std::endl;
        it = si.find(2);
        if (it == si.end())
            std::cout << "find(2) = false" << std::endl;
    }

    void test_unordered_map() {
        std::cout << "\ntest std::unordered_map" << std::endl;

        //std::unordered_map<int, std::string> dict = {{1, "one"}, {2, "two"}};
        //dict.insert({3, "three"});
        //dict.insert(std::make_pair(4, "four"));
        //dict.insert({{4, "another four"}, {5, "five"}});

        std::unordered_map<int, std::string> um;
        std::cout << "um.bucket_count = " << um.bucket_count() << std::endl;
        int count = 2000;
        um.max_load_factor(5);
        int i = 0;

        auto populate_um = [&i, &um](int count) {
            std::cout << "populate_um(" << count << ")" << std::endl;
            while (count--) {
                um.insert(std::make_pair<int&, std::string>(++i, std::to_string(++i)));
                //um.insert(std::make_pair(count, std::to_string(count)));
            }
            std::cout << "um.max_load_factor = " << um.max_load_factor() << std::endl;
            std::cout << "um.bucket_count = " << um.bucket_count() << std::endl;
            std::cout << "um.load_factor = " << um.load_factor() << std::endl;
        };

        populate_um(0);
        populate_um(1000);
        populate_um(1000);
        populate_um(1000);
        populate_um(1000);
        populate_um(1000);
        populate_um(1000);
    }

    template <typename T>
    void fff(T s) {
        std::cout << s << '\n';
    }

    template <typename T>
    void printTupleInfo(T& t) {
        std::cout << "\ntuple info: " << std::endl;
        std::cout << "tuple size = " << std::tuple_size_v<std::remove_reference<decltype(t)>::type> << std::endl;
        //std::cout << "tuple size = " << std::tuple_size_v<decltype(t)> << std::endl;
        std::cout << "[0] : " << std::get<0>(t) << std::endl;
        std::cout << "[1] : " << std::get<1>(t) << std::endl;
        std::cout << "[0] type: " << type2name<std::tuple_element_t<0, std::remove_reference<decltype(t)>::type>>() << std::endl;
        std::cout << "[1] type: " << type2name<std::tuple_element_t<1, std::remove_reference<decltype(t)>::type>>() << std::endl;
        std::cout << "[0] type decltype(std::get<0>(t)) : " << type2name<decltype(std::get<0>(t))>() << std::endl;
        std::cout << "[1] type decltype(std::get<0>(t)) : " << type2name<decltype(std::get<1>(t))>() << std::endl;
    }

    void test_tuple() {
        std::cout << "\ntest std::tuple" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        std::tuple<int, std::string> t = {1, "_1_"};
        printTupleInfo(t);
        int i1 = 11;
        int j1 = 22;
        // std::tie - создает кортеж из ссылок, параметры ссылки на lvalue
        auto t1 = std::tie(i1, j1);
        printTupleInfo(t1);

        std::cout << "\ntest 2" << std::endl;
        using t_t           = std::tuple<int, std::string>;
        std::vector<t_t> vt = {{4, "_4_"}, {1, "_1_"}, {2, "_2_"}, {3, "_3_"}, {3, "0_3_"}};
        auto print_vt       = [](std::vector<t_t>& vt) {
            using t_t = std::tuple<int, std::string>;
            std::for_each(vt.begin(), vt.end(), [](t_t& t) { std::cout << std::get<0>(t) << "," << std::get<1>(t) << std::endl; });
        };
        std::sort(vt.begin(), vt.end(), [](auto&& t1, auto&& t2) { return t1 < t2; });
        print_vt(vt);

        std::cout << "\ntest 3 sort by firts,second value" << std::endl;
        std::sort(vt.begin(), vt.end(), [](auto&& t1, auto&& t2) { return std::tie(std::get<0>(t1), std::get<1>(t1)) < std::tie(std::get<0>(t2), std::get<1>(t2)); });
        print_vt(vt);

        std::cout << "\ntest 4 sort by second,first value" << std::endl;
        std::sort(vt.begin(), vt.end(), [](auto&& t1, auto&& t2) { return std::tie(std::get<1>(t1), std::get<0>(t1)) < std::tie(std::get<1>(t2), std::get<0>(t2)); });
        print_vt(vt);

        std::cout << "\ntest 5 (futures)" << std::endl;
        auto [i, j, k] = std::tuple{1, 2, 3};
        auto t5        = std::make_tuple(i, std::ref(j), k);
        auto t6        = std::tuple{1, std::ref(j), 3};
        static_assert(std::is_same_v<decltype(t5), std::tuple<int, int&, int>>);
        static_assert(std::is_same_v<decltype(t6), std::tuple<int, std::reference_wrapper<int>, int>>);

        std::cout << type2name(0) << std::endl;
        std::cout << type2name(&i) << std::endl;
        std::cout << type2name(std::ref(i)) << std::endl;

        std::cout << "\ntest 6 std::tuple_cat" << std::endl;
        auto t61 = std::make_tuple(1, "2");
        auto t62 = std::make_tuple(3, "4");
        auto t63 = std::tuple_cat(t61, t62);
        std::cout << "tuple size = " << std::tuple_size_v<decltype(t63)> << std::endl;
        std::cout << std::get<0>(t63) << "," << std::get<1>(t63) << "," << std::get<2>(t63) << "," << std::get<3>(t63) << "," << std::endl;
    }

}