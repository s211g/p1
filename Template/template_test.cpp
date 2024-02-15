#include <type_traits>
#include <string>
#include <vector>

#include "template_test.hpp"
#include "TStack.hpp"

namespace template_test {

    class ABase1 {
    public:
        void f(int) { std::cout << "ABase1:f(int)" << std::endl; }
        void g(int) { std::cout << "ABase1:g(int)" << std::endl; }
    };

    class ABase2 {
    public:
        void g(char) { std::cout << "ABase2:g(char)" << std::endl; }
    };

    class A : public ABase1, public ABase2 {
    public:
        void f(int*) { std::cout << "A:fx(int*)" << std::endl; }
    };

    void test1() {
        std::cout << "\ntest1" << std::endl;

        // Переопределение функций базового класса
        // если в производном классе переопределена функция(тоже имя но аргументы могут быть и другими),
        // то доступ к функциям базового класса становица возможным только через пространство имени родительского класса
        int i;
        A a;
        // a.f(1);  // не доступна, тк в производном функция переопределена
        a.ABase1::f(i);
        a.f(&i);

        // a.g(1); // неопределенность, пересечение имени фунции в базовых классах
        // но есть доступ через пространство имен
        a.ABase1::g(1);
        a.ABase2::g('a');
    }

    template <typename T>
    class AT {
    public:
        int a;
        int a1;
        void f(int) { std::cout << "A<T>:f(int)" << std::endl; }
    };

    class B {
    public:
        int b;
        int b1;
        void f(char) { std::cout << "B:f(char)" << std::endl; }
    };

    template <typename T>
    class CT : public AT<T>, public B {
    public:
        using AT<T>::a; // для доступа к переменным базового шаблонного класса, если класс не шаблонный то using не требуется
        using AT<T>::f;
        using B::f; // для доступа требуется using, тк есть переопределенная функция и есть неоднозначность имен функций базовых классов

        void f() {
            std::cout << "CT::f()" << std::endl;

            AT<T>::a = 0; // доступ через пространство имен
            a        = 0; // надо использовать using
            //a1       = 0; // нет доступа
            b = 0; // базовый класс не шаблонный поэтому using не требуется
        }
    };

    void test2() {
        std::cout << "\ntest2" << std::endl;

        // чтобы использовать функции совместно с переопределенными без пространства надо использовать:
        // using BaseClass::FnName;
        // разрешает использование фунций базовых классов без указания пространств имен(+ делает функцию pablic если класс унаследован закрыто)
        // если конешно она в базовом была public
        // вытаскивает все фнкции независимо от сигнатур

        CT<int> c;
        c.f();

        // доступ через пространство имен
        c.AT::f(1);
        c.B::f('a');

        // доступны фунции и переменные базовых классов т.к. использовали using
        c.f(1);
        c.f('a');
        c.a = 0;
        c.b = 0;

        // вывод:
        // CT::f()
        // A<T>:f(int)
        // B:f(char)
        // A<T>:f(int)
        // B:f(char)

        // ??? есть ли вывод аргументов по возвращаемому значению
    }

    class Person {
    public:
        std::string name;

        template <typename T,
                  typename = std::enable_if_t<
                      !std::is_base_of<Person, std::decay_t<T>>::value &&
                      !std::is_integral<std::remove_reference_t<T>>::value>>
        explicit Person(T&& name_) :
            name(std::forward<T>(name_)) {
            static_assert(
                std::is_constructible_v<std::string, T>);

            std::cout << "Person::Person(T&&)" << std::endl;
        }

        Person(const Person&) { std::cout << "Person::Person(const Person&)" << std::endl; }
        Person(Person&&) { std::cout << "Person::Person(Person&&)" << std::endl; }
        Person(int idx) :
            name(std::to_string(idx)) { std::cout << "Person::Person(int)" << std::endl; }
    };

    void test_enable_if() {
        std::cout << "\ntest std::enable_if" << std::endl;
        // is_base_of<T1, T2>::value - является ли Т1 базовым для Т2
        // is_integral<T>::value - является ли тип интегральным
        // decay<T> - убирает все ссылки и квалификаторы volatile и const (+превращает массивы и функции в указатели)
        // is_constructible<T1, T2> - можно ли создать Т1 из Т2

        // в классе Person исключают вызов шаблонного конструктора в случае если параметр является(унаследован от) Person или интегральным значением
        std::cout << "\ntest 1" << std::endl;
        Person p1(1);
        std::cout << "\ntest 2" << std::endl;
        Person p2("123");
        std::cout << "\ntest 3" << std::endl;
        Person p3(std::string("123"));
        std::cout << "\ntest 4" << std::endl;
        Person p4(p1);
        std::cout << "\ntest 5" << std::endl;
        Person p5(std::move(p1));
        std::cout << "\ntest 6" << std::endl;
        //Person p6(u"123"); // выдаст статический ассерт is_constructible() что объяснит почему нельзя вызывать с таким аргументом
    }

    template <typename T>
    class C {
    public:
        C(T t_) :
            t(t_) {}
        friend std::ostream& operator<<(std::ostream& os, const C<T>& c) {
            os << c.t;
            return os;
        }

    private:
        T t;
    };

    void test_friend() {
        std::cout << "\ntest friend function" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        C c1(1);
        std::cout << c1 << std::endl;
        C c2(std::string("abc"));
        std::cout << c2 << std::endl;
    }
}
