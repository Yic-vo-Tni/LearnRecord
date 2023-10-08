//
// Created by lenovo on 10/7/2023.
//
#include "iostream"

struct x{
    void f(){
        std::cout << "6\n";
    }

    void f3(){
        std::cout << "9\n";
    }

//    void f(int ){
//        std::cout << "int\n";
//    }

    //---------------------------------------------------------1.2

    template<typename T>
    auto operator ->*(T v){
        return (this->*v)();
    }
    //operator.* 不可以重载 operator->* 可以重载
};

void f2(void(x::*p)(), x& x){
    (x.*p)();
}

void f4(void(x::*p)(), x* x){
    (x->*p)();
}
//使用.*还是->*取决于你的表达式左操作数是指针还是引用或普通实例

//------------------------------------------------------------------- 1.1

void f6(void(x::*p)(), x* x, int n){
    (x->*p)();
}
//f6(&x::f, &x, 5);
//out put: 6
void f7(void(x::*p)(int), x* x, int n){
    (x->*p)(n);
}
//f7(&x::f, &x, 5);
//out put: int

//----------------------------------------------------------------------- 1.2



int main(){
    x x;
    f2(&x::f, x);
    f2(&x::f3, x);
    f4(&x::f, &x);
    //f7(&x::f, &x, 5);

    //---------------------------------------------------------------------------1.2

    x->*& x::f; //overload
}