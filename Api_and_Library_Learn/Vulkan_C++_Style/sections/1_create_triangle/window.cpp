//
// Created by lenovo on 10/13/2023.
//

#include "window.h"

#include <utility>
#include <stdexcept>

namespace yic{
    Window::Window(int w, int h, std::string name)  : width_(w), height_(h), name_(std::move(name)) {
        initWindow();
    }

    Window::~Window() {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    void Window::initWindow() {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window_ = glfwCreateWindow(width_, height_, name_.c_str(), nullptr, nullptr);

    }

    void Window::createSurface(vk::Instance instance, vk::SurfaceKHR* surface) {
        VkSurfaceKHR temSurface;
        if(glfwCreateWindowSurface(instance, window_, nullptr, &temSurface) != VK_SUCCESS){
            throw std::runtime_error("failed to create window surface");
        }
        *surface = temSurface;
    }


}