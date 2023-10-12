//
// Created by lenovo on 10/12/2023.
//
#include "iostream"

namespace yic{
    template<typename T>
    void swap(T&& x, T&& y){
        //往引用的方向去推理，推理T是左值引用还是右值引用，保证往引用上面去推
//        T   // ? 给ta初始化 output 4, 4  == int& c = x; c是x的一个别名
//        c = x;
//        x = y;
//        y = c;   error

//---------------------------------
  //      auto c = x;
        typename std::remove_reference<T>::type c = x; //去掉引用类型的，把T自身的引用去掉 typename说明他是一个类型
        x = y;
        y = c;
    }
}

int main(){
    int x = 3, y = 4;
    yic::swap(x, y); //& && 奇数的引用
    std::cout << x << ", " << y << std::endl;

    yic::swap(1, 2);

    return 0;
}