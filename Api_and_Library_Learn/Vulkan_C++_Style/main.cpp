//
// Created by lenovo on 10/13/2023.
//
#include <iostream>

#include "src/application.h"

int main(){
    yic::Application app;

    try {
        app.run();
    } catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}