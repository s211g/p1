#include <iostream>
#include <memory>

template <typename T> class X {
  public:
    void f() { static_cast<T *>(this)->f(); }
};

class A : public X<A> {
  public:
    void f() { std::cout << "hi" << std::endl; }
};

int main() {
    A a;
    a.f();
    return 0;
}
