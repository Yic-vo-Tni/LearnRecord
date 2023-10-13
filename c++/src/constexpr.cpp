//
// Created by lenovo on 10/13/2023.
//

#include "iostream"

constexpr int a = 23;//全局不占内存，局部占内存
const int b = 23;//也一样，直接存在代码里，代码空间

class miku{
public:
    constexpr miku(){};
};

int main(){
    int n;
    std::cin >> n;
    constexpr miku miku;

    const int x = 123 + n; //运行时
    constexpr int y = 245 /*+ n*/; //编译时确定值


    int* p = const_cast<int *>(&x);
    *p = 0;
    std::cout << x << std::endl; //output 123
    std::cout << *p << std::endl; //output 0


    return 0;
}