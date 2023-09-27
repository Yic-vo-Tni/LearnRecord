//
// Created by lenovo on 9/25/2023.
//
#include "iostream"
#include "tuple"

struct Person{
    std::string Name;
    int Age;
};

std::tuple<std::string , int> CreatePerson(){
    return {"yic", 20};
}
//std::pair<std::string, int>
//std::tuple<>,可以包含多个参数

int main(){
//    auto person = CreatePerson();
//    std::string& name =  std::get<0>(person);
//    int age = std::get<1>(person);
//
//    std::tie(name, age) = CreatePerson();

    auto[name, age] = CreatePerson();
    //c++ 17

    return 0;
}