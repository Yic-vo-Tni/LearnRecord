//
// Created by lenovo on 9/25/2023.
//
#include "iostream"
#include "variant"
//单一变量存储多种类型数据
//相比union更加安全，但union更节省内存

enum class ErrorCode{
    None = 0,
    NotFound = 1,
    NoAccess = 2
};

std::variant<std::string , ErrorCode> Read_file(){
    return {};
}
//比起optional可以给我们更多的信息



int main(){
    std::variant<std::string , int> data;
    std::cout << sizeof(int ) << std::endl;
    data = "yic";
    std::cout << sizeof(std::string) << std::endl;
    std::cout << std::get<std::string >(data) << '\n';
    data = 2;
    std::cout << sizeof(data) << std::endl;
    std::cout << std::get<int >(data) << '\n';

//?????
    return 0;
}