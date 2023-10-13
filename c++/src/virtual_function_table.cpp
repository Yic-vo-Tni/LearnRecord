//
// Created by lenovo on 10/13/2023.
//
#include "iostream"

class miku{
public:
    virtual void run(){
        std::cout << "miku run" << std::endl;
    }
    virtual void jump(){
        std::cout << "miku jump" << std::endl;
    }
};
class yic : public miku{
    void run() override {
        std::cout << "yic run" << std::endl;
    }
    void jump() override {
        std::cout << "yic jump" << std::endl;
    }
};

typedef void(*fun)();

int main(){
    std::cout << sizeof(miku) << std::endl; // output 1, add virtual output 8
    //单个虚函数为8，多个虚函数依旧是8,相当于指针指向了一个指针数组
    //std::cout << sizeof(void) << std::endl; // output 1

    miku miku;
    yic yic, yic_1;

    std::cout << *(void**)&yic << std::endl;
    std::cout << *(void**)&yic_1 << std::endl;
    //same

    (*(fun **)&yic)[0]();
    //((void(***)())&yic_1)[0]();
    //(*yic_1)();


    return 0;
}