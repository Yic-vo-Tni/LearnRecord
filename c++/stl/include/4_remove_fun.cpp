//
// Created by lenovo on 10/7/2023.
//
#include "vector"
#include "list"
#include "print.h"
#include "algorithm"

template<typename Tv, typename Tc>
void remove_value(Tc& c, const Tv& v){
    auto remove_it = std::remove(c.begin(), c.end(), v);
    c.erase(remove_it, c.end());
}

int main(){
    std::vector v{1, 2, 3, 4, 5};
    print(v);
    ::remove_value(v, 1);
    print(v);
    std::erase(v, 5);
    print(v);
    std::erase_if(v, [](int i){return i % 2 != 0;});
    print(v);
}