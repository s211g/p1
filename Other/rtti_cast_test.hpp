#pragma once

#include <iostream>

namespace rtti_cast_test {

    /*
reinterpret_cast < type-id > ( expression )
Позволяет преобразовывать любой указатель в указатель любого другого типа. 
Также позволяет преобразовывать любой целочисленный тип в любой тип указателя и наоборот.

static_cast <type-id> ( expression )
Преобразует выражение в тип type-id, основываясь только на типах, присутствующих в выражении.
Оператор static_cast можно использовать для таких операций, как преобразование указателя в 
базовый класс в указатель на производный класс. Такие преобразования не всегда являются безопасными.
В стандартном языке C++, проверка типа во время выполнения не выполняется, что обеспечивает безопасность преобразования. 

dynamic_cast < type-id > ( expression )
Преобразует операнд expression в объект типа type-id.

для ссылок:
    try {
        C &c = dynamic_cast<C&>(a);
        printf_s("in GlobalTest\n");
    }
    catch(std::bad_cast) {
        printf_s("Can't cast to C\n");
    }

Операторы dynamic_cast и static_cast перемещают указатель по иерархии классов. Однако зависит исключительно от информации, 
static_cast предоставленной в операторе приведения, и поэтому может быть небезопасным. 


RTTI — это механизм динамической идентификация типов (Run Time Type Identification — RTTI). 
Это обозначает, что типы объектов можно опознавать уже после компиляции и запуска программы. 
Многими устаревшими реализациями компиляторов С++ механизм RTTI не поддерживается. 
В некоторых компиляторах имеются опции, позволяющие включать или отключать RTTI. 

    const type_info& ti = typeid(A);
    //type_info {
    //bool operator== (const typeinfo &ob) const;
    //bool operator!= (const typeinfo &ob) const;
    //bool before(const typeinfo &ob) const;
    //const char *name() const;
*/

    void test_type_info();

    inline void test() {
        std::cout << "rtti cast tests" << std::endl;

        test_type_info();
    }
}
