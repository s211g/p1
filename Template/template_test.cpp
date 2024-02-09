#include <type_traits>
#include <string>
#include <vector>

#include "template_test.hpp"
#include "TStack.hpp"

namespace template_test {

    template <
        template <typename> class ContainerIn,
        typename T>
    int ContainerPush(ContainerIn<T>& container, T value) {
        container.push_back(value);
        return container.size();
    }

    template <
        typename T,
        template <typename> class ContainerIn>
    int ContainerPush2(ContainerIn<T>& container, T value) {
        container.push_back(value);
        return container.size();
    }

    // template <
    //     typename T,
    //     template <typename Elem, typename = std::allocator<Elem>> class ContainerInT, // определение некого типа контейнера
    //     typename ContainerIn = ContainerInT<T>>                                       // определение контейнера с указанным параметром
    template <
        typename T,
        template <typename Elem = T, typename = std::allocator<T>> class ContainerIn // определение типа контейнера, поскольку есть значения по умолчанию
                                                                                     // то в сигнатуре функции можно не указывать параметры шаблона
        >
    int ContainerPush3(ContainerIn<>& container, T value) {
        container.push_back(value);
        ContainerIn<> co;
        return container.size();
    }

    template <
        typename T,
        typename C>
    int ContainerPush4(C& container, T value) {
        container.push_back(value);
        return container.size();
    }

    template <
        typename T,
        typename C>
    class CContainerPush4 {
    public:
        CContainerPush4(C& container, T value) {}
    };


    void test_template_template_parameters() {
        std::cout << "\ntest template template parameter" << std::endl;

        // внутри параметров шаблона можно также указать параметр шаблон, формат:
        // template <typename, typename ....> typename
        // каждое typename может быть проименовано
        // template <typename T1> typename
        // чтобы в дальнейшем можно было например конкретизировать второй параметр шаблона
        // template <typename T1, typename = std::allocator<T1>> typename
        // можно задать имя которое можно дальше использовать
        // template <typename T1, typename T2= std::allocator<T1>, typename = T2> typename
        // если typename T3 = T4, а Т4 предполагает что должен быть параметр то в коде потом надо { T4<...> }
        // ??? если typename T6 = T4<T5>, тут Т6 уже определен с параметром и в коде уже используется просто { Т6 t }
        // !!!???? почемуто для т6 в аргументе надо объявлять T6<>, вероятно и в теле функции
        // !!! при создании объекта после типа класса надо <> а при вызове функции можно не указывать <>

        std::vector<int> v;
        auto s = ContainerPush<std::vector>(v, 1);

        // далее параметры шаблона выводятся из аргументов
        s = ContainerPush(v, 2);
        s = ContainerPush<>(v, 3);

        s = ContainerPush2<int>(v, 4);
        s = ContainerPush2<>(v, 5);
        s = ContainerPush2(v, 6);

        s = ContainerPush3<>(v, 7);
        s = ContainerPush3(v, 8);
        //s = ContainerPush3(v, double(8)); // поскольку тип контейнера и тип value связаны(должны быть одного типа), не скомпилируется

        s = ContainerPush4(v, 9);
        s = ContainerPush4(v, double(10)); // !!! T - double, тип контейнера и тип значения не связаны

        // автоматически вывод типа параметров шаблона
        CContainerPush4 cc(v, 0);

        std::cout << "Container size : " << s << std::endl;
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
    class A {
    public:
        A(T t_) :
            t(t_) {}
        friend std::ostream& operator<<(std::ostream& os, const A<T>& a) {
            os << a.t;
            return os;
        }

    private:
        T t;
    };

    void test_friend() {
        std::cout << "\ntest friend function" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        A a1(1);
        std::cout << a1 << std::endl;
        A a2(std::string("abc"));
        std::cout << a2 << std::endl;
    }

    void test_template_parameters() {
        std::cout << "\ntest template parameters of temlate" << std::endl;

        std::cout << "\ntest 1" << std::endl;
        TStack<double> s1;
        TStack<int> s2;
        s1.push(1.1);
        s1.push(2.2);
        s1.push(3.3);
        s2 = s1;
        while (!s2.empty()) {
            std::cout << " " << s2.top();
            s2.pop();
        }
    }
}
