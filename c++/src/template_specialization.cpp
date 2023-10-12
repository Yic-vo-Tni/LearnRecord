//
// Created by lenovo on 10/12/2023.
//
#include "iostream"


template <typename T, typename U>
auto add(T t, U u) -> decltype(t + u){
    return t + u;
}

//指针类型进行特殊处理，特化
template<typename T, typename U>
auto add(T* t, U* u) -> decltype(*t + *u){
    return *t + *u;
}//全特化

template<>
int add(int t, int u){
    return t - u;
}//function specialization

//----------------------------------------------------------fun

template<typename T, typename U>
class Miku{
public:
    Miku(){
        std::cout << "normal template" << std::endl;
    }
};

template<>
class Miku<int, double>{
public:
    Miku(){
        std::cout << "specialization template" << std::endl;
    }
};

template<typename T>
class Miku<T , int >{
public:
    Miku(){
        std::cout << "special template <T, int> " << std::endl;
    }
};

//-----------------------------------------------------------------------class temp sp_li

template<typename T>
class NiLu{
public:
     static std::string miku;

};
template<typename T>
std::string NiLu<T>::miku = "y";
template<>
std::string NiLu<double>::miku = "n";



int main(){
//    std::cout << add(1, 2) << std::endl;

//    int m = 1, n = 2;
//    int* p_m = &m, * p_n = &n;
//    std::cout << add(p_m, p_n) << std::endl;

    //----------------------------------------------------------------fun tem

//    Miku<int, double> miku;
//    Miku<double, int> miku_;
//    Miku<std::string, int> miku_s;

    //----------------------------------------------------------------class temp sp-li

    std::cout << NiLu<double>::miku << std::endl;
    std::cout << NiLu<int>::miku << std::endl;

    return 0;
}