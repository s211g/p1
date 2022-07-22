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
    }

    void test_sum14() {
        std::cout << "test sum c++14" << std::endl;
        auto s = sum(1, 3, 5);
        std::cout << "= " << s << std::endl;
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
}