//
// Created by lenovo on 10/11/2023.
//
#include "iostream"

//int add(int& a, int& b){
//    return a + b;
//}
//
//int add_(int a, int b){
//    a = 1;
//    b = 2;
//    return a + b;
//}

//template <typename T, typename U>
//decltype(T() + U()) add(T t, U u){
//    return t + u;
//}

class A{
    int x, y;
public:
    A() = delete;
    A(int a) : x(a){}

    A& operator+(const A& a){
        this->x += a.x;
        return *this;
    }

    int getx() const { return x; }


};

std::ostream& operator << (std::ostream& ostream, const A& a){
    return ostream << a.getx();
}

template <typename T, typename U>
auto add(T t, U u) -> decltype(t + u){
    return t + u;
} //???

//--------------------------------------------------------------------------
//class___template
//template <typename T>
class Print{
public:

//    Print& operator()(T t){
//        //在类模板中定义成员函数时，通常不需要显式添加 `&`，这句话并不完全正确||不使用引用，每次调用都会创建一个新的临时对象，而不是对同一个对象进行修改。这是为什么不需要使用引用来保持对象的持久性
//        //每次调用都是在不同的临时对象上进行的，所以不需要使用引用来保持对象的持久性
//        //不论 `operator()` 是否带有 `&`，都能正常输出，因为每次调用都是在不同的对象上进行的
//        std::cout << t << std::endl;
//        return *this;
//    }

    template <typename U>
    Print& operator()(U u){
        std::cout << u << std::endl;
        return *this;
    }
};


int main(){


//    Print<int> print;
//    print(3)(4)(4);
//    Print<std::string> s_print;
//    s_print("yic")("ttt");

    Print print;
    print(3)(4)(5)("yic")(2.34);

    return 0;
}

//    std::cout << sizeof(add(1, 2)) << std::endl;
//    std::cout << sizeof(add(2.2f, 3.1)) << std::endl;
//
//    decltype(1 + 2) a;
//    std::cout << typeid(a).name() << std::endl;
//
//    A x(2), y(3);
//    std::cout << add(x, y) << std::endl;
//    std::cout << A(x) + A(y) << std::endl;
//
//    A z(4);
//    A result = add(z, x);
//    std::cout << result.getx() << std::endl;

//------------------------------------------------------------------