#include "delete_test.hpp"
#include "Utils.hpp"

// void operator delete(void* p, size_t size) noexcept {
//     std::cout << "delete " << p << " size " << size << std::endl;
// }

// void operator delete(void* p) noexcept {
//     std::cout << "delete " << p << std::endl;
// }

// void operator delete[](void* p, size_t size) noexcept {
//     std::cout << "delete[] " << p << " size " << size << std::endl;
// }

// void operator delete[](void* p) noexcept {
//     std::cout << "delete[] " << p << std::endl;
// }

namespace delete_test {

    class A {
    public:
        long long i1{1};
        long long i2{2};
        ~A() { std::cout << "A::~A()" << std::endl; }
    };

    class B : public A {
    public:
        long long j{3};
        long long k{4};
    };

    void test_delete_class() {
        std::cout << "test_delete_class" << std::endl;

        A* a = new B();

        delete a;
    }

    void test_delete_array() {
        std::cout << "test_delete_array" << std::endl;

        constexpr int s = 0x5;
        A* a            = new A[s];

        //delete a;
        // вывод:
        // A::~A()
        // delete 0x1eb095c6c18 size 8

        //delete[] a;
        // вывод:
        // A::~A()
        // A::~A()
        // A::~A()
        // delete[] 0x1eb095c6c10 size 32
        // вызывается деструктор для каждого объекта, потом удаление всей памяти разом

        uint8_t* offset = reinterpret_cast<uint8_t*>(a);
        utils::dump("array a", offset - 8, sizeof(A) * s + 8, 8, true, true);

        // array a(0x25ee4100750) size = 88 :
        // 0x0000025EE4100750 + 0        : 05 00 00 00 00 00 00 00
        // 0x0000025EE4100758 + 8        : 01 00 00 00 00 00 00 00
        // 0x0000025EE4100760 + 16       : 02 00 00 00 00 00 00 00
        // 0x0000025EE4100768 + 24       : 01 00 00 00 00 00 00 00
        // 0x0000025EE4100770 + 32       : 02 00 00 00 00 00 00 00
        // 0x0000025EE4100778 + 40       : 01 00 00 00 00 00 00 00
        // 0x0000025EE4100780 + 48       : 02 00 00 00 00 00 00 00
        // 0x0000025EE4100788 + 56       : 01 00 00 00 00 00 00 00
        // 0x0000025EE4100790 + 64       : 02 00 00 00 00 00 00 00
        // 0x0000025EE4100798 + 72       : 01 00 00 00 00 00 00 00
        // 0x0000025EE41007A0 + 80       : 02 00 00 00 00 00 00 00
    }

}