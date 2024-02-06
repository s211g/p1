#include <iostream>

#include "inheritance_test.hpp"

namespace inheritance_test {

class A{
public:
    void f1(const A& a)
    {
            std::cout << "A::f1() a.i_priv = " << a.i_priv << std::endl;
            std::cout << "A::f1() a.i_prot = " << a.i_prot << std::endl;
    }

    virtual void f_v(const A& a) {};
    virtual void f_v() {};

private:    
    int i_priv{0};
protected:    
    int i_prot{0};
};

class B : public A{
    
    public:
    void f2(const A& a){
            // нет доступа из потомка к данным другого объекта
            //std::cout << "B::f2() a.i_priv = " << a.i_priv << std::endl;
            //std::cout << "B::f2(const A& a) a.i_prot = " << a.i_prot << std::endl;
    }

    void f2(const B& b){
            // есть доступ
            std::cout << "B::f2(const B& b) b.i_prot = " <<  b.i_prot << std::endl;
            // нет доступа
            //std::cout << "B::f2(const B& b) b.i_priv = " <<  b.i_priv << std::endl;
    }

    void f3(){
            // нет доступа к приватным данным базового класса
            //std::cout << "B::f3() i_priv = " << i_priv << std::endl;
            
            // есть доступ к защищенным данным базового класса
            std::cout << "B::f3() i_prot = " << i_prot << std::endl;
    }

    virtual void f_v(const A& a) override {
            // у виртуальной функции также нет доступа из потомка к данным другогр объекта
            //std::cout << "B::f2() a.i_priv = " << a.i_priv << std::endl;
            //std::cout << "B::f2() a.i_prot = " << a.i_prot << std::endl;
    }

    virtual void f_v() override {
            // нет доступа к приватным данным базового класса
            //std::cout << "B::f3() i_priv = " << i_priv << std::endl;            
            // есть доступ к защищенным данным базового класса
            std::cout << "B::f3() i_prot = " << i_prot << std::endl;
    }

};

    void test_1() {
        std::cout << "\ntest 1" << std::endl;

        // protected - доступ из иерархии потомков 
        // private - доступ только тот уровень на котором переменная(функция определена), потомки не имеют доступа

        // B::f3() 
        // функции потомка тогоже объекта имеют доступ только к protected членам

        // B::f1() 
        // protected,private !!! объекты класса-потомка имеют доступ только через фунции этого уровня иерархии где объявлена переменная
        // (тоже относится к переопределенным виртуальным ф-м)

        // B::f2() 
        // функции объекта класса потомка имеют доступ к приватному члену другого объекта объявленого в базовом классе 

    A a1;
    A a2;
    std::cout << "\n---------------------" << std::endl;
    a2.f1(a1);

    B b;
    B b2;
    std::cout << "\n---------------------" << std::endl;
    b.f3();
    std::cout << "\n---------------------" << std::endl;
    b.f2(b2);

    std::cout << "\n---------------------" << std::endl;
    std::cout << "b.f1(const A& a)" << std::endl;
    b.f1(a1);
    }

    class D {
        private:
        int j_priv;
    };

    class C : private A, public D {
public:
    using A::i_prot;
    //using D::j_priv; // нельзя тк в базовом прописан private

    void f3(){
            // есть доступ, в закрытом наследовании если использовать using
            std::cout << "B::f3() i_prot = " << i_prot << std::endl;
    }

    };

     void test_2() {
        std::cout << "\ntest 2" << std::endl;

        // используя using можно разприватить закрытое наследование, но нельзя разприватить приватные секции базового класса
        C c;
        c.f3();
    }
 } 
