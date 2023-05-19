#include "virtual_function_test.hpp"

namespace virtual_function_test {

    void dump(uint8_t* data, uint32_t size) {
        char buff[23];
        for (int i = 0; i < size; ++i) {
            sprintf(buff, "%02X", (int)data[i]);
            //std::cout << (int)data[i] << " ";
            //std::cout << buff << " ";
            std::cout << ((i && (!(i % 8))) ? "\n" : "") << buff << " ";
        }
        std::cout << std::endl;
    }

    void test1() {
        std::cout << "virtual function test1" << std::endl;

        std::cout << " sizeof(void*) " << sizeof(void*) << std::endl;
        std::cout << "sizeof C >" << sizeof(C) << std::endl;

        C c;
        C* pc = &c;
        B* pb = dynamic_cast<B*>(&c);
        A* pa = dynamic_cast<A*>(&c);

        try {
            auto& sa = dynamic_cast<A&>(c);
            std::cout << "cast ok" << std::endl;
        }
        catch (...) {
            std::cout << "cast failed" << std::endl;
        }

        if (dynamic_cast<BA*>(&c) == nullptr)
            std::cout << "error cast to BA* : return nullptr" << std::endl;

        try {
            auto& sba = dynamic_cast<BA&>(c);
            std::cout << "cast ok" << std::endl;
        }
        catch (std::bad_cast& ex) {
            std::cout << "error cast to BA& : exception std::bad_cast" << std::endl;
        }

        // в случае ошибки
        // каст на указатель возвращает nullptr
        // каст на ссылку генерирует std::bad_cast

        std::cout << " pc " << pc << " pb " << pb << " pa " << pa << std::endl;
        std::cout << " c " << &pc->c << " b " << &pb->b << " a " << &pa->a << std::endl;

        std::cout << "pc->f()" << std::endl;
        pc->f();
        std::cout << "pb->f()" << std::endl;
        pb->f();
        std::cout << "pa->f()" << std::endl;
        pa->f();

        // расположение данных внутри объекта класса С :
        // вложенности у базовых классов нет
        /*
        {
        data B 0000000CDFAFF848 pc,pb -> vptrB
               0000000CDFAFF850          b
        data A 0000000CDFAFF858 pa    -> vptrA
               0000000CDFAFF860          a
        data C 0000000CDFAFF868          c      
        }
        */

        // данные идут по порядку наследования слева направо(class 1: public 2,3,4 - данные внутри объекта идут: 2,3,4,1)
        // !!!тела конструкторов вызываются в этойже последовательности 2,3,4,1
        // !!! из конструкторов(и из деструкторов) таблица на указатели не работает, вызываются реализация функций этого класса или базовых
        // в начале кадого блока(данных класса) идет vptr для этого класса
        // данные производного класса идут последними и ему vptr не надоё
    }

    void test2() {
        std::cout << "virtual function test2" << std::endl;

        std::cout << " sizeof(void*) " << sizeof(void*) << std::endl;
        std::cout << "sizeof C_BA >" << sizeof(C_BA) << std::endl;

        C_BA cba;
        C_BA* pcba = &cba;
        BA* pba    = dynamic_cast<BA*>(&cba);
        A* pa      = dynamic_cast<A*>(&cba);
        std::cout << " pcba " << pcba << " pba " << pba << " pa " << pa << std::endl;
        std::cout << " c " << &pcba->c << " b " << &pba->b << " a " << &pa->a << std::endl;

        //dump(reinterpret_cast<uint8_t*>(pcba), sizeof(cba));
        dump((uint8_t*)pcba, sizeof(cba));

        std::cout << "pcba->f()" << std::endl;
        pcba->f();
        std::cout << "pba->f()" << std::endl;
        pba->f();
        std::cout << "pa->f()" << std::endl;
        pa->f();

        // расположение данных внутри объекта класса С :
        /*
        {
        class BA { data A  00000014A1F0F6F8 pcba,pba vptrBA,vptrA
                           00000014A1F0F700          a
                   data BA 00000014A1F0F708          b
                 }
                 data CBA  00000014A1F0F710          c 
        }
        */
    }

    void test3() {
        std::cout << "virtual function test3" << std::endl;
        BA ba;
        A* pa = dynamic_cast<A*>(&ba);
        pa->getThis();
        // !!! вызовется переопределенная BA::getThis() хотя сигнатуры виртуальных функций разные
        // случай, когда возвращаемый тип метода - это указатель на сам класс
        pa->A::getThis();
        // можно напрямую указать какой метод вызывать
    }
}