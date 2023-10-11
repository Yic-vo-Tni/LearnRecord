//
// Created by lenovo on 10/9/2023.
//
#include <miku.h>

class Yic : public miku::Application{
public:
    Yic(){

    }

    ~Yic(){

    }
};

miku::Application* miku::createApplication() {
    return new Yic();
}