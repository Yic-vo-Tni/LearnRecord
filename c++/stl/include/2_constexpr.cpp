//
// Created by lenovo on 10/7/2023.
//
#include "iostream"
#include "vector"

constexpr auto f(){
    std::vector<int>v{1, 3, 4, 5};
    return v;
}

int main(){
    //constexpr auto n = f();
    //报错，去掉constexpr
    constexpr auto n = f().size();
}