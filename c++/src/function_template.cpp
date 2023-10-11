//
// Created by lenovo on 10/11/2023.
//
#include "iostream"

int add(int& a, int& b){
    return a + b;
}

int add_(int a, int b){
    a = 1;
    b = 2;
    return a + b;
}

template <typename T, typename U>
decltype(T() + U()) add(T t, U u){
    return t + u;
}

int main(){
    std::cout << sizeof(add(1, 2)) << std::endl;
    std::cout << sizeof(add(2.2f, 3.1)) << std::endl;

    decltype(1 + 2) a;
    std::cout << typeid(a).name() << std::endl;

    return 0;
}