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
#include "camera.h"

#include <string>
#include <iostream>
#include "functional"

namespace yic {
    static Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

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
        static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn, int width, int height);
        static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);


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
