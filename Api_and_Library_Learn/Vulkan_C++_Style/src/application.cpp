//
// Created by lenovo on 10/13/2023.
//
#include "application.h"

namespace yic {


    void Application::run() {

        while (!window.windowShouldClose()) {
            glfwPollEvents();

            renderer.drawFrame();
        }

        device.device_().waitIdle();
    }


}