#include <type_traits>
#include <string>
#include <vector>

#include "template_explicit_specialization_test.hpp"


namespace template_explicit_specialization_test_ {
    void call_f() {
        std::cout << "\ncall_f() : " << std::endl;
        using namespace template_explicit_specialization_test;
        f(1);
        f(double());
        f(std::string());
        f(char());
    }
}

namespace template_explicit_specialization_test {

    // ---------------- шаблон класса -------------------

    // статический член
    template <>
    int X<double>::s = 1;

    // явная специализация только одной функции общегошаблона
    template <>
    void X<float>::f() {
        std::cout << "X<float>::f()" << std::endl;
    }

    // ---------------- шаблон класса(явная специализация) -------------------

    // "template<>" не требуется
    X<int>::X() {
        std::cout << "X<int>::X()" << std::endl;
    }

    // "template<>" не требуется
    void X<int>::f() {
        std::cout << "X<int>::f()" << std::endl;
    }


    // ---------------- шаблон класса -------------------

    // ---------------- шаблон функции -------------------

    // определение через свободную функцию, будет конфликтовать с явной специализацией "template <> void f<double> {}"
    void f(double t) {
        std::cout << "f(double t)" << std::endl;
    }

    // cпециализация от общего шаблона
    template <>
    void f<std::string>(std::string t) {
        std::cout << "f<std::string>(std::string t)" << std::endl;
    }

    // cпециализация от общего шаблона, Т выводится из аргумента
    // поэтому можно опустить после имени специализацию в скобках: f<>(...) или f<char>(...)
    template <>
    void f(char t) {
        std::cout << "f<>(char t)" << std::endl;
    }

    // ---------------- шаблон функции -------------------

    void test_explicit_outside_definitions() {
        std::cout << "\n test_explicit_outside_definitions" << std::endl;

        // при полной(явной) специализации "template<>" либо перед явно специализированным классом(в .hpp), либо перед явно специализированной функцией(в .cpp)
        // "template<>" перед реализацией члена класса вне класса не надо если есть объявление полной специализации этого класса:
        // template<> class A<TYPE>{...}

        std::cout << "\n commmon template class: " << std::endl;
        // общий шаблон
        X<double> x;
        x.f();
        x.f1();
        x.ft<int>();
        x.ft<char>();
        x.b.f();
        std::cout << "x.s : " << x.s << std::endl;
        // вывод:
        // X<T>::X()
        // X<T>::f()
        // X<T>::ft()
        // X<T>::ft()

        std::cout << "\n commmon template class + specialization f() : " << std::endl;
        // общий шаблон, с одной явно специализированной функцией
        X<float> x1;
        x1.f();
        x1.f1();
        x1.ft<int>();
        std::cout << "x1.s : " << x1.s << std::endl; // не откомпилируется если нет определения(в сpp или общего для всех типов в hpp)
        // вывод:
        // X<T>::X() - общий шаблон
        // X<float>::f() - специализированная функция общего шаблона
        // X<T>::ft() - общий шаблон

        std::cout
            << "\n full specialization template class: " << std::endl;
        // явно специализированный шаблон
        X<int> x2;
        x2.f();
        // x2.f1(); - в явной специализации нет такой функции
        x2.ft<int>();
        x2.ft<char>();
        // вывод:
        // X<int>::X()
        // X<int>::f()
        // X<int>::ft()
        // X<int>::ft()

        std::cout << "\n struct specialization : " << std::endl;
        X<double>::D d_double;
        d_double.f();
        X<float>::D d_float;
        d_float.g();
        //X<int>::D d_int; - в полной специализации X<int> такого типа нет
        // вывод:
        // X<T>::D::f() общий шаблон
        // X<float>::D::g() явно специализированная структура общего шаблона(другое содержимое, другие переменные, другие функции)

        std::cout << "\n function definitions f() : " << std::endl;
        f(1);
        f(double());
        f(std::string());
        f(char());
        // вывод:
        // f<T>(T t) общий шаблон
        // f(double t) переопределено через свободную функцию, если не объявить ее в заголовке то может вызваться шаблонная "ft<T>(T t)
        // f<std::string>(std::string t)  специализация
        // f<>(char t) специализация, с автоматическим выводом Т из аргумента


        // notice:
        // !!! перед возвращаемым типом можно написать inline и реализацию перенести в заголовок

        // "template <>" сообщает что дальнейшее определение класса или функции относится к явной специализации
        // и после имени фукции можно не писать ни <Тип> ни <>

        // в заголовке надо объявить сигнатуры явных специализаций и свободных функций

        // Явная специализация не может быть friend declaration

        // перед определением статических членов(в .cpp) впереди необходимо template<>
        // в определении нельзя использовать "()" иначе определение превратиться в объявление

        // в явно специализинованном классе все содержимое определятеся лиш тем что определено именно в нем

        template_explicit_specialization_test_::call_f(); // если в заголовке не объявлены все специализации и сигнатуры свободных функций вывод может не совпасть
    }

    template <typename T1, typename T2>
    class A {
    public:
        A() {
            std::cout << "A<T1, T2>::A()" << std::endl;
        }
        A(T1 t1, T2 t2) {
            std::cout << "A<T1, T2>::A(t1,t2)" << std::endl;
        }

        int sum(T1 t1, T2 t2) {
            std::cout << "A<T1, T2>::sum(t1,t2)" << std::endl;
            return t1 + t2;
        }
    };

    template <>
    class A<int, char> {
    public:
        A();
        // A() {
        //     std::cout << "A<int, char>::A()" << std::endl;
        // }

        A(int t1, int t2) {
            std::cout << "A<int, char>::A(int,char)" << std::endl;
        }

        template <typename Tx, typename Ty>
        int sum(Tx t1, Ty t2) {
            std::cout << "A<int, char>::sum(t1,t2)" << std::endl;
            return t1 + static_cast<int>(t2);
        }

        void f();
    };

    // определение членов вне класса
    //template<> - не требуется
    A<int, char>::A() {
        std::cout << "A<int, char>::A()" << std::endl;
    }
    void A<int, char>::f() {}

    void test_explicit_specialization() {
        std::cout << "\n test_explicit_specialization" << std::endl;

        A<int, int> a1;
        a1.sum(1, 2);
        //A<int, char> a2;
        //a2.sum(1, 2);

        // выводт конкретизации из аргументов
        A a3(1, 2); // A<T1, T2>::A(t1,t2)
        char c = 1;
        A a4(1, c); // A<int, char>::A(int,char)
        double d;
        A a5(1, d); // A<T1, T2>::A(t1,t2)
    }

    template <typename _T1, typename _T2>
    P<typename std::decay_t<_T1>,
      typename std::decay_t<_T2>>
    make_p(_T1&& t1, _T2&& t2) {
        typedef typename std::decay_t<_T1> type1;
        typedef typename std::decay_t<_T2> type2;
        //return P<type1, type2>(std::forward<_T1>(t1), std::forward<_T2>(t2));
        return P<type1, type2>();
    }

    template <template <typename...> class C, typename... T>
    auto make_x1(T&&... t) {
        return C<typename std::decay_t<T>...>();
    }

    template <template <typename...> class C, typename... T>
    auto make_x2(T&&... t) {
        return C<T...>();
    }

    // подставляет параметры шаблона в зависимости от типа переменных в аргументе
    template <template <typename...> class C, typename... T>
    //auto
    C<typename std::decay_t<T>...>
    make_x(T&&... t) {
        return C<typename std::decay_t<T>...>(std::forward<T>(t)...);
    }

    void test_partial_specialization() {
        std::cout << "\n test_partial_specialization" << std::endl;

        P<int, char> p1;
        P<char, int> p2;
        P<int, int> p3; // если не будет специализации P<int, int> то будет конфликт между <T1,int> и <int,T2>
        //P<char, char> p4; // ошибка, неполный тип, т.к. нет реализации обобщенного шаблона
        P<int*, int> p5;
        P<int&, int> p6;
        P<int*, int*> p7;
        // вывод:
        // P<int, T2>::P()
        // P<T1, int>::P()
        // P<int, int>::P()
        // P<T*, int>::P()
        // P<T1&, int>::P()
        // P<T*, T*>::P()

        char c{};
        int i{};
        int& si = i;

        std::cout << "make_p() ---------------" << std::endl;
        {
            auto p1 = make_p(i, c);
            auto p2 = make_p(c, i);
            auto p3 = make_p(i, i);
            auto p5 = make_p(&i, i);
            auto p6 = make_p(si, i); // ссылка на int воспринимается как int
            auto p7 = make_p(&i, &i);
        }

        auto xxx = std::make_pair(i, i);

        // вывод:
        // P<int, T2>::P()
        // P<T1, int>::P()
        // P<int, int>::P()
        // P<T*, int>::P()
        // P<int, int>::P()
        // P<T*, T*>::P()

        std::cout << "make_x1() ---------------" << std::endl;
        {
            auto p1 = make_x1<P>(i, c);
            auto p2 = make_x1<P>(c, i);
            auto p3 = make_x1<P>(i, i);
            auto p5 = make_x1<P>(&i, i);
            auto p6 = make_x1<P>(si, i); // ссылка на int воспринимается как int
            auto p7 = make_x1<P>(&i, &i);
        }
        // вывод:
        // P<int, T2>::P()
        // P<T1, int>::P()
        // P<int, int>::P()
        // P<T*, int>::P()
        // P<int, int>::P()
        // P<T*, T*>::P()

        std::cout << "make_x2() ---------------" << std::endl;
        {
            auto p1 = make_x2<P>(i, c);
            // auto p2 = make_x2<P>(c, i);
            // auto p3 = make_x2<P>(i, i);
            // auto p5 = make_x2<P>(&i, i);
            // auto p6 = make_x2<P>(si, i); // ссылка на int воспринимается как int
            // auto p7 = make_x2<P>(&i, &i);
        }
        // вывод:
        // P<T&, T&>::P() -  // !!! внутри не используется decay_t() передаем инт,
        // свертка универсальнной ссылки дает аргумент T&, и передаваемый параметр неявно кастица к T&

        std::cout << "make_x() ---------------" << std::endl;
        auto f = make_x<F>(1, std::string("abc"));
        // вывод:
        //F<T1,T2>(1, abc)

        std::cout << "auto ---------------" << std::endl;
        // вывод типа по типам аргуметов конструктора
        // !!! в обобщенном шаблоне и в специализированном должна быть возможность вывода типа,
        // т.е. если хотим выводить по аргументу типа Т/T*/T&, то в обобщенном и в специализированном должен быть консруктор (T/T*/T&)
        E e1(i);
        E e2(si);
        E e3(&i);
        E e4(c);
        E e5(&c);
        // вывод:
        // E<int>::E(int&)
        // E<int>::E(int&)
        // E<int>::E(int*)
        // E<T>::E(T&)
        // E<T>::E(T*)

        // ??? нет конфликта с E<int*>, даже если поставить определение вперед E<int> все равно сводится к E<int>
    }
}
