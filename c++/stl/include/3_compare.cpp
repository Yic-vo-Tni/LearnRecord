//
// Created by lenovo on 10/7/2023.
//
#include "iostream"

template<class T, class U>
constexpr bool cmp_less(T t,U u)noexcept{
    using UT = std::make_unsigned_t<T>;
    using UU = std::make_unsigned_t<U>;
    if constexpr (std::is_signed_v <T> == std::is_signed_v<U>)
        return t < u;
    else if constexpr (std::is_signed_v<T>){
        return t < 0 || UT(t) < u;
        //return t < 0 ? true : UT(t) < u;
    } else {
        return u >= 0 && t < UU(u);
    }
}

int main(){
    std::cout << std::boolalpha << (5u < -1) << "\n";
    std::cout << std::boolalpha << ::cmp_less(5u, 1) << "\n";
    std::cout << std::boolalpha << ::cmp_less(5u, 2u) << "\n";
    std::cout << std::boolalpha << ::cmp_less(5u , -1) << "\n";
}