#include <iostream>
#include <array>
#include <vector>
#include <set>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <utility>

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

}
