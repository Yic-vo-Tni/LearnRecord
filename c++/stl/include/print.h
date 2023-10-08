//
// Created by lenovo on 10/7/2023.
//

#ifndef LEARN_RECORD_PRINT_H
#define LEARN_RECORD_PRINT_H

#include "ranges"
#include "map"
#include "format"

template <typename... Args>
inline void print(const std::string_view fmt_str, Args&&... args){
    auto fmt_args{std::make_format_args(args...)};
    std::string outstr{std::vformat(fmt_str, fmt_args)};
    fputs(outstr.c_str(), stdout);
}

inline void print(std::ranges::range auto v){
    print("size:{} ", v.size());
    print("[{} ", v.size());
    for(const auto& i : v) print("{} ", i);
    print("]\n");
}

template<typename T, typename T2>
inline void print(const std::map<T, T2>& map){
    print("size:{} ", map.size());
    for(auto&[k, v] : map) print("{}:{}", k, v);
    print("\n");
}

#endif //LEARN_RECORD_PRINT_H
