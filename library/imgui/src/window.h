//
// Created by lenovo on 9/28/2023.
//

#ifndef LEARN_RECORD_WINDOW_H
#define LEARN_RECORD_WINDOW_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "yic_define.h"

#include <string>
#include <iostream>

namespace yic {

    class Window {
    public:
        Window(int w, int h, std::string name);
        ~Window() = default;


        GLFWwindow* getWindow(){return window;}
        bool windowShouldClose(){ return glfwWindowShouldClose(window);}

        void processInput(GLFWwindow* window);
        void imguiDraw();

    private:
        void initWindow();
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

        GLFWwindow* window;
        std::string name{};
        int width{};
        int height{};

        std::string text{"Text"};
        char textbox[50]{"text box"};
        ImVec4 color{};
    };

}

#endif //LEARN_RECORD_WINDOW_H
