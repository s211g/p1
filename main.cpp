#include <iostream>
#include <memory>
#include <functional>

#include "mutex_test.hpp"
#include "smartpointer_test.hpp"

void f() {
    std::cout << "f() NULL" << std::endl;
}

void f(std::string& s) {
    std::cout << "f(std::string)" << s << std::endl;
}

template <typename... T>
void f(std::string& s, T&&... t) {
    std::cout << "1 " << s << std::endl;
    f(std::forward<T>(t)...);
}

int test_template_var_arg() {
    std::cout << "h i" << std::endl;
    std::string a = "a";
    std::string b = "b";
    std::string c = "c";

    f(a, b, c);
    return 0;
}

int main() {
    //mutex_test::test();
    smartpointer_test::test();
    return 0;
}
