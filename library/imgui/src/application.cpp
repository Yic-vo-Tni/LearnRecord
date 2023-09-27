//
// Created by lenovo on 9/28/2023.
//

#include "application.h"

namespace yic{

    void Application::run() {

        while (!yic_window.windowShouldClose()){
            yic_window.processInput(yic_window.getWindow());

            render.setBuffer();

            yic_window.imguiDraw();

            glfwSwapBuffers(yic_window.getWindow());
            glfwPollEvents();
        }
    }


}