//
// Created by lenovo on 9/28/2023.
//

#include "application.h"

namespace yic {

    void Application::run() {
        glEnable(GL_DEPTH_TEST);

        while (!yic_window.windowShouldClose()) {
            auto currentFrame = static_cast<float>(glfwGetTime());
            yic_window.deltaTime = currentFrame - yic_window.lastFrame;
            yic_window.lastFrame = currentFrame;

            yic_window.processInput(yic_window.getWindow());

            render.render(width, height);

            yic_window.imguiDraw();

            glfwSwapBuffers(yic_window.getWindow());
            glfwPollEvents();
        }
    }

}