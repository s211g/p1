#include <iostream>
#include <functional>
#include <map>
#include <vector>

#include "typedeclaration_test.hpp"
#include "TypeUtils.hpp"

namespace typedeclaration_test {

    class A {
    public:
        A() { std::cout << "A::A()" << std::endl; }
        ~A() { std::cout << "A::~A()" << std::endl; }
        A(int i) { std::cout << "A::A(int)" << std::endl; }
        int fn(int i) {
            std::cout << "A::fn(" << i << ")" << std::endl;
            return 0;
        }
    };

    void test_array() {
        std::cout << "\ntest array" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        int a1[10];
        int a2[10];
        swap_array(a1, a2);

        std::cout << "\ntest 2" << std::endl;
        A* a = new A[10];
        //delete a; //- assert
        delete[] a;

        std::cout << "\ntest 3" << std::endl;
        int* i = new int[10];
        delete i; // no asserts ???
        std::cout << "---" << std::endl;

        std::cout << "\ntest 4" << std::endl;
        int i4[10];
        std::cout << "sizeof i4 = " << sizeof(i4) << std::endl;
        std::cout << "sizeof A = " << sizeof(A) << std::endl;

        std::cout << "\ntest 5" << std::endl;
        int m[10];
        // !!! в аргументе имя массивва превращается в указатель
        auto f5 = [](int m[10]) {
            std::cout << "fn(int m[10]), sizeof(m) = " << sizeof(m) << ", m type: " << type_utils::type2name<decltype(m)>() << std::endl;
        };
        std::cout << "int m[10], sizeof(m) = " << sizeof(m) << ", m type: " << type_utils::type2name<decltype(m)>() << std::endl;
        f5(m);
        // вывод:
        //int m[10], sizeof(m) = 40, m type: int[10]
        //fn(int m[10]), sizeof(m) = 8, m type: int*
    }

    int fn1(int i, double d) {
        std::cout << "fn1() " << i << " " << d << std::endl;
        return 0;
    }
    int fn2(int i, double d) {
        std::cout << "fn2() " << i << " " << d << std::endl;
        return 0;
    }
    int fn3(int i, double d) {
        std::cout << "fn3() " << i << " " << d << std::endl;
        return 0;
    }

    void test_ptr_to_fn() {
        std::cout << "\ntest fn ptr" << std::endl;

        std::cout << "\ntest pointer to fn" << std::endl;
        // ptr to fn
        // тип (*имя_указателя) (параметры);
        int (*p_fn)(int, double);
        p_fn = &fn1;
        p_fn(1, 2.3);
        (****p_fn)(1, 2.3);

        std::cout << "\ntest pointer to fn[3]" << std::endl;
        // ptr to array fn[]
        // тип (*имя_указателя[size]) (параметры);
        int (*p_fnarr[3])(int, double) = {fn1, fn2, fn3};
        p_fnarr[0](1, 1.1);
        p_fnarr[1](2, 2.2);
        p_fnarr[2](3, 3.3);

        // ptr to class member fn
        // retval (ClassName::*PtrName) (Parmeters);
        //int (A::*p_A_fn)(int) = A::fn;  - ERROR
        int (A::*p_A_fn)(int) = &A::fn;
        A a;
        (a.*p_A_fn)(1);
        (a.*p_A_fn)(1);

        // shared_ptr:
        //((*instance).*f)(std::forward<Args>(args)...);
        //(instance.get()->*f)(std::forward<Args>(args)...);
    }

    A a();
    A a(int);

    void test_declaration_fn() {
        std::cout << "\ntest declaration fn" << std::endl;
        a();
        a(1);
    }

    A a() {
        std::cout << "call A a()" << std::endl;
        return A{};
    }
    A a(int) {
        std::cout << "call A a(int)" << std::endl;
        return A{};
    }

    int* f(int) {
        std::cout << "f()" << std::endl;
        return (int*)nullptr;
    }

    void test_declaration_lambda() {
        std::cout << "\ntest make lambda" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        auto make_lambda = [](int param) { return [param](int value) { return value + param; }; };
        auto l1          = make_lambda(1);
        auto l2          = make_lambda(2);
        std::cout << l1(1) << std::endl;
        std::cout << l2(1) << std::endl;

        std::cout << "\ntest 2 internal state lambda" << std::endl;
        int i = 0;
        // mutable разрешает менять внутриние переменные переданые в кв скобках как копии
        auto l11 = [i]() mutable { std::cout << ++i << std::endl; };
        l11();
        l11();
        std::cout << "i = " << i << std::endl;
        auto l12 = [&i]() { std::cout << ++i << std::endl; };
        l12();
        l12();
        std::cout << "i = " << i << std::endl;

        std::cout << "\ntest 3" << std::endl;
        // объявляется как
        // int*(int)
        // а кастица как
        // int* (*)(int)
        std::function<int*(int)> l3 = f;
        l3(1);
        int* (*pl3)(int) = *l3.target<int* (*)(int)>();
        if (!pl3)
            std::cout << "pl3 == nullptr" << std::endl;
        else
            (*pl3)(0);

        std::cout << "\ntest 4" << std::endl;
    }


    template <typename C>
    auto f0_ta(C&& c) {
        return (c[0]);
    }

    template <typename C>
    decltype(auto) f1_ta(C&& c) {
        return c[0];
    }

    template <typename C>
    auto f2_ta(C&& c) {
        return std::forward<C>(c)[0];
    }

    template <typename C>
    auto f3_ta(C&& c) -> decltype(std::forward<C>(c)[0]) {
        return std::forward<C>(c)[0];
    }

    template <typename C>
    decltype(auto) f4_ta(C&& c) {
        return std::forward<C>(c)[0];
    }

    template <typename T>
    decltype(auto) f5_ta(T&& t) {
        return std::forward<T>(t);
    }

    void test_auto() {
        std::cout << "\ntest auto" << std::endl;

        // чтобы переменная auto была в точности такого же типа как ее инициализатор то надо объявлять ее:
        // decltype(auto) var = ...
        // чтобы функция(auto f()) возвращала ссылку есть 2 варианта:
        // 1 спецификатор возвращаемого значения:
        // auto f() -> ... { ... }
        // 2 объявить функцию  decltype(auto) f() { return reference; } и возвратить тип ссылки
        // PS: чтобы возвратить ссылку на некую переменную из функции(не явл ссылкой) надо возвратить ее в скобках:
        // return (var);
        // PS: для универсальных ссылок пользовать std::forward<>()

        std::vector<int> v{1, 2, 3};
        std::cout << "> " << type_utils::type2name<decltype(v[0])>() << std::endl;
        auto printv = [&] {for(const auto& i: v) std::cout << i << " "; std::cout<<std::endl; };

        std::cout << "\ntest 0" << std::endl;

        int i{0};
        decltype(auto) a00 = i;
        std::cout << "a00 type : " << type_utils::type2name<decltype(a00)>() << std::endl;

        decltype(auto) a0 = f0_ta(v);
        a0                = 10;
        std::cout << "a0 type : " << type_utils::type2name<decltype(a0)>() << std::endl;
        printv();

        decltype(auto) a1 = f1_ta(v);
        a1                = 11;
        std::cout << "a1 type : " << type_utils::type2name<decltype(a1)>() << std::endl;
        printv();

        decltype(auto) a2 = f2_ta(v);
        a2                = 12;
        std::cout << "a2 type : " << type_utils::type2name<decltype(a2)>() << std::endl;
        printv();

        decltype(auto) a3 = f3_ta(v);
        a3                = 13;
        std::cout << "a3 type : " << type_utils::type2name<decltype(a3)>() << std::endl;
        printv();

        decltype(auto) a4 = f4_ta(v);
        a4                = 14;
        std::cout << "a4 type : " << type_utils::type2name<decltype(a4)>() << std::endl;
        printv();

        decltype(auto) a5 = f5_ta(1);
        std::cout << "a5 type : " << type_utils::type2name<decltype(a5)>() << std::endl;

        // вывод:
        //a00 type : int
        //a0 type : int
        //1 2 3
        //a1 type : int&
        //11 2 3
        //a2 type : int
        //11 2 3
        //a3 type : int&
        //13 2 3
        //a4 type : int&
        //14 2 3

        std::cout << "\ntest 1" << std::endl;
        // в случае функций предположительно которые возвращают proxy, которые инициализируют переменную объявленную как auto
        // лучше делать статик каст:
        std::vector<bool> v2{true, false, true};
        auto b = static_cast<bool>(v[0]); // v[0] возвращает vector<bool>::reference
        // auto b = v[0]; создаст переменную (не ссылку) типа vector<bool>::reference
        // и есть непределенность как эта переменная будет преобразована в bool если ее передать в какуюнито функцию f(b)

        std::cout << "\ntest 2 [](auto){}" << std::endl;
    }


    struct AB {
        //AB() { std::cout << "AB::AB()" << std::endl; }
        int a{1};
        int b{2};
        std::string s{"abc"};
        void print() { std::cout << "a = " << a << ", b = " << b << ", s : " << s << std::endl; }
    };

    struct CD : public AB {
        CD(std::initializer_list<int>) {
            std::cout << "CD::CD(std::initializer_list<int>)" << std::endl;
        }
        CD(int, int) {
            std::cout << "CD::CD(int, int)" << std::endl;
        }
        CD(int, int, std::string) {
            std::cout << "CD::CD(int, int, std::string)" << std::endl;
        }
    };

    void test_bracket() {
        std::cout << "\ntest bracket" << std::endl;

        auto printv = [](auto& v) {for(const auto& i: v) std::cout << i << " "; std::cout<<std::endl; };

        std::cout << "\ntest 0" << std::endl;
        int x1(0);
        int x2 = 0;
        int x3{0};
        int x4 = {0};

        // не путать с объявлением функции f type - int(void):
        int f1(); // объявление функции
        std::cout << "f1 type - " << type_utils::type2name<decltype(f1)>() << std::endl;
        int f2{}; // переменная
        std::cout << "f2 type - " << type_utils::type2name<decltype(f2)>() << std::endl;

        std::cout << "\ntest 1" << std::endl;
        std::vector<int> v1{3, 5};
        std::cout << "v1{4, 5} : ";
        printv(v1);
        std::vector<int> v2(4, 5);
        std::cout << "v1(4, 5) : ";
        printv(v2);

        std::cout << "\ntest 2 std::initializer_list<int>" << std::endl;
        auto il1 = {1.1, 2.2, 3.3};
        printv(il1);
        std::cout << "il1 type - " << type_utils::type2name<decltype(il1)>() << std::endl;
        auto il2 = {1, 2, 3};
        printv(il2);
        std::cout << "il2 type - " << type_utils::type2name<decltype(il2)>() << std::endl;
        // нельзя задавать разные типы
        //auto il3 = {1, 2.2, 3.3};

        std::cout << "\ntest 3 structure initialisation" << std::endl;
        AB ab1;
        ab1.print();
        AB ab2 = {3, 4}; // если нет конструктора то инициализируются поэлементно
        ab2.print();
        AB ab3{5, 6, "def1"};
        ab3.print();
        AB ab4 = {7, 8, "def2"};
        ab4.print();
        //AB ab5(7, 8, "def2"); // !!! нет такого конструктора

        std::cout << "\ntest 4 structure initialisation" << std::endl;
        CD cd1{1, 2};        // не смотря что есть конструктор CD(int, int) предпочтение отдается CD(std::initializer_list<int>)
        CD cd2{3, 4, "def"}; // CD(int, int, std::string)
        CD cd3{3, 4, 5};     // CD(std::initializer_list<int>)
    }


    template <typename T>
    struct MyMapV_typedef {
        typedef std::map<T, std::vector<T>> type;
    };
    template <typename T>
    using MyMapV_using = std::map<T, std::vector<T>>;

    void test_using() {
        std::cout << "\ntest using" << std::endl;

        std::cout << "\ntest 0" << std::endl;
        // using typedef
        typedef std::map<int, std::vector<int>> mapv_typedef;
        typedef void (*pfn_typedef)(int);
        // using using
        using mapv_usging = std::map<int, std::vector<int>>;
        using pfn_using   = void (*)(int);

        // temlates using typedef
        // !!! перед зависимыми(зависят от параметра шаблона) типами надо указывать typename
        typename MyMapV_typedef<int>::type m1;
        m1[1].push_back(1);
        // temlates using using
        MyMapV_using<int> m2;
        m2[1].push_back(1);

        //PS: в заголовках определены псевдонимы для таких функций например:
        //template <class _Ty>
        //using remove_const_t = typename remove_const<_Ty>::type;
    }

    void test_enum() {
        std::cout << "\ntest enum" << std::endl;

        enum class E : std::uint8_t {
            red   = 1,
            blue  = 3,
            green = 5
        };

        std::cout << "\ntest 0" << std::endl;
        //std::cout << E::red << std::endl; // error
        // toUType(E::red) преобразует в тип uint
        std::cout << type_utils::toUType(E::red) << std::endl; // error
    }

    class A3 {
    public:
        A3() = default;
        template <typename T>
        A3(const T&) {
            std::cout << "A3::A3(const T&)" << std::endl;
        }
    };

    void test_default() {
        std::cout << "\ntest default" << std::endl;

        // рекомендации о большой тройке, если объявлен хоть один из:
        // - дестркуктор
        // - копирующий конструктор
        // - оператор присваивания
        // то должны быть объявлени и остальные два
        class A {
        public:
            ~A()        = default; // ! по умолнчнию noexcept
            A(const A&) = default;
            A& operator=(const A&) = default;
        };

        std::cout << "\ntest 1" << std::endl;
        // перемещающие операции(констр перемещ и присваивание перемещением)
        // генерируются если не объявлены явно
        // 1) пеермещающие операции
        // 2) или деструктор
        // 3) или копирующие операции(констр коп и присваивание коп)
        class A1 {
        public:
            A1() { std::cout << "A1::A1()" << std::endl; }
            ~A1() { std::cout << "A1::~A1()" << std::endl; }
            A1(const A1&) { std::cout << "A1::A1(const A1&)" << std::endl; }
            A1& operator=(const A1&) {
                std::cout << "A1::operator=()" << std::endl;
                return *this;
            }
        };
        A1 a11;
        A1 a1(std::move(a11)); // конструктора перемещения не генерируется, кастица к конструктору копирования A(const A&)

        std::cout << "\ntest 2" << std::endl;
        // копирующие операции (констр копирования и присваивание копированием)
        // генерируются если не объявлены явно
        // 1) пеермещающие операции
        class A2 {
        public:
            A2() { std::cout << "A2::A2()" << std::endl; }
            A2(A2&&) { std::cout << "A2::A2(A1&&)" << std::endl; }
        };
        A2 a22;
        //A2 a2(a22); - ошибка, конструктор копирования не сгенерен, тк есть конструктор перемещения

        std::cout << "\ntest 3" << std::endl;
        // Шаблоны функций-членов не подавляют генерацию специальных функций
        A3 a33;
        A3 a3(a33); // вызывается A3::A3(const A3&) - сгенерирвана автоматически, хотя есть шаблонная - A3::A3(const T&)

        std::cout << "\ntest 4" << std::endl;
        A3 a4(a22); // вызывается шаблонная - A3::A3(const T&)
    }

    void test_return() {
        std::cout << "\ntest constructor and inference return values from functions" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        using B = type_utils::B;
        auto f  = [](B&& b) {
            using B = type_utils::B;
            std::cout << "type b : " << type_utils::type2name<decltype(b)>() << std::endl;
            //B b0(b); // ошибка !!! хоть b имеет тип B&& - тоесть r-value но без move(b) его конструктору как r-value не передать
            B b1(std::move(b));
            std::cout << "-----" << std::endl;
            B b2(B{2});
            std::cout << "b2.i = " << b2.i << std::endl;
        };

        f(B{1});

        std::cout << "\ntest 2" << std::endl;
        B b3{B{3, 4}}; // временный объект B{3, 4} и является объектом b3, никакого конструктора для b3 не вызывается
        std::cout << "b3.i = " << b3.i << std::endl;
        auto f3 = [](B& b) {
            return std::move(b);
        };

        std::cout << "\ntest 3" << std::endl;
        B b4(f3(b3)); // а вот тут уже вызывается B::B(B&&)

        std::cout << "\ntest 31" << std::endl;
        auto f31 = []() {
            B b{31};
            return b; // !!! для возвращаемых локальных переменных оптимизируется как return std::move(b);
        };
        B b31{f31()};

        std::cout << "\ntest 4" << std::endl;
        B b5(B(B(B(B(4))))); // вызывается только один конструктор B::B(int) i = 4

        std::cout << "\ntest 5" << std::endl;
        B b6(B(B(B(B(5)))), 0); // сначала вызывается конструктор B::B(int) i = 5, потом B::B(B&&, int)

        std::cout << "\ntest 6" << std::endl;
        using A = type_utils::A;
        auto f2 = [](int i) {
            return A{i};
        };
        A a0(0);
        A a1(f2(4)); // объект создается функцией f2() и онже является а1, !!! никакого конструктора для a1 не вызывается
        //A a2(std::move(a0)); // ошибка, тк конструктор перемещения удален
    }
}