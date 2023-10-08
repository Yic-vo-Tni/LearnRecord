//
// Created by lenovo on 10/7/2023.
//
#include "iostream"
#include "string_view"
#include "format"

template<typename... Args>
void print(const std::string_view fmt_str, Args &&...args) {
    auto fmt_args{std::make_format_args(args...)};
    std::string outstr{std::vformat(fmt_str, fmt_args)};
    fputs(outstr.c_str(), stdout);//效率比cout要高
}
//报错原因：gcc截止目前11.2版本仍未完全支持c++20新特性，而clang/msvc可以更完美的支持c++20的新特性，故能成功运行
//建议：

//template<typename... Args>
//void print(const std::string_view fmt_str, Args&&... args) {
//    std::string outstr = std::format(fmt_str, std::forward<Args>(args)...);
//    fputs(outstr.c_str(), stdout);
//}
//

struct Frac {
    int a, b;
};

template<>
struct std::formatter<Frac> {
    template<class ParseContext>
    constexpr auto parse(ParseContext &context) {
        return context.begin();
    }

    template<class FormatContext>
    auto format(const Frac &f, FormatContext &context) {
        return std::format_to(context.out(), "{0:d}/{1:d}", f.a, f.b);
    }
};

int main() {
    //print("Hello, {}!\n", "world");

    Frac frac{5, 3};
    print("{}", frac);  //output 5/3
    //msvc正常运行，gcc报错
}