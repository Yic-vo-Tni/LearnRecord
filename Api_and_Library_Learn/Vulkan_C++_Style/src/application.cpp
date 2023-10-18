//
// Created by lenovo on 10/13/2023.
//
#include "application.h"

namespace yic {


    void Application::run() {
        //pipeline.
        while (!window.windowShouldClose()) {
            glfwPollEvents();
        }

    }


}