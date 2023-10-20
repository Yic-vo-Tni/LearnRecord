//
// Created by lenovo on 10/13/2023.
//

#ifndef LEARN_RECORD_WINDOW_H
#define LEARN_RECORD_WINDOW_H

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "glfw3.h"
#include "string"

#include "vulkan/vulkan.hpp"


namespace yic{

class Window{
public:
    Window(int w, int h, std::string name);
    ~Window();

    Window(const Window&) = delete;
    Window& operator = (const Window&) = delete;

    void initWindow();
    GLFWwindow* getWindow() { return window_;}
    bool windowShouldClose(){return glfwWindowShouldClose(window_);}

    void createSurface(vk::Instance instance, vk::SurfaceKHR* surface);

private:
    GLFWwindow* window_;
    const int width_;
    const int height_;
    std::string name_;
};

}

#endif //LEARN_RECORD_WINDOW_H
