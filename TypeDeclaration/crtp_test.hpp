#pragma once

#include <iostream>

namespace crtp_test {

    template <class Derived>
    class addable {
    public:
        auto operator+(const Derived& rhs) const {
            Derived lhs = static_cast<const Derived&>(*this);
            lhs.i += rhs.i;
            return lhs;
        }
    };

    template <class Derived>
    class addable_f {
    public:
        friend auto operator+(Derived lhs, const Derived& rhs) {
            lhs.i += rhs.i;
            return lhs;
        }
    };

    //    class A : public addable<A> {
    class A : public addable_f<A>, public addable<A> {
    public:
        A(int i_) :
            i(i_) {}
        int i;
    };

    class A2 : public addable<A2> {
    public:
        A2(int i_) :
            i(i_) {}
        int i;
    };


    void test_1();

    inline void test() {
        std::cout << "crtp tests" << std::endl;
        test_1();
    }
}