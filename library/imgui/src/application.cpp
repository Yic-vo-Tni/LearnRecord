//
// Created by lenovo on 9/28/2023.
//

#include "application.h"

namespace yic {

    void Application::run() {
        //glEnable(GL_DEPTH_TEST);

        while (!yic_window.windowShouldClose()) {
            yic_window.processInput(yic_window.getWindow());

            render.render();

            yic_window.imguiDraw();

            glfwSwapBuffers(yic_window.getWindow());
            glfwPollEvents();
        }
    }

}