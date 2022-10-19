#include <type_traits>
#include <string>

#include "template_test.hpp"

namespace template_test {

    class Person {
    public:
        std::string name;

        template <typename T,
                  typename = std::enable_if_t<
                      !std::is_base_of<Person, std::decay_t<T>>::value &&
                      !std::is_integral<std::remove_reference_t<T>>::value>>
        explicit Person(T&& name_) :
            name(std::forward<T>(name_)) {
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
    }
}
