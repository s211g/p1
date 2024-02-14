#include <type_traits>
#include <string>
#include <vector>

#include "template_template_specialization_test.hpp"
#include "TStack.hpp"

namespace template_template_specialization_test {

    // функции ---------------------------------------------------

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

    // классы ---------------------------------------------------

    template <
        typename T,
        typename C>
    class CContainerPush5 {
    public:
        CContainerPush5(C& container, T value) {}
    };

    template <
        typename T,
        template <typename Elem = T, typename = std::allocator<T>> class ContainerIn // определение типа контейнера, поскольку есть значения по умолчанию
                                                                                     // то в сигнатуре функции можно не указывать параметры шаблона
        >
    class CContainerPush6 {
    public:
        CContainerPush6(ContainerIn<>& container, T value) {}
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

        std::cout << "Container size : " << s << std::endl;

        // автоматически вывод типа параметров шаблона
        CContainerPush5 cc5(v, 0);
        CContainerPush6 cc6(v, 0);
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
