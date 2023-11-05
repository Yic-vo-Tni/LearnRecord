//
// Created by lenovo on 10/21/2023.
//
#include "window_window.h"
#include "Log.h"

#include "Event/ApplicationEvent.h"
#include "Event/KeyEvent.h"
#include "Event/MouseEvent.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace miku {

    static bool glfwInitialized = false;

    static void glfwErrorCallback(int error, const char* description){
        YIC_CORE_ERROR("GLFW error ({0}): {1}", error, description);
    }

    Window* Window::Create(const miku::WindowProps &props) {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const miku::WindowProps &props) {
        init(props);
    }

    WindowsWindow::~WindowsWindow(){
        shutdown();
    }

    void WindowsWindow::init(const miku::WindowProps &props) {
        data_window.width = props.Width;
        data_window.height = props.Height;
        data_window.name = props.Title;

        YIC_CORE_INFO("Create window {0} {1} {2}", props.Title, props.Width, props.Height);

        if(!glfwInitialized){
            int success = glfwInit();
            YIC_CORE_ASSERT(success, "Could not initialize glfw");
            glfwSetErrorCallback(glfwErrorCallback);
            glfwInitialized = true;
        }

        window = glfwCreateWindow((int)props.Width, (int)props.Height, data_window.name.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(window);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        YIC_CORE_ASSERT(status, "Failed to initialize glad!");
        glfwSetWindowUserPointer(window, &data_window);
        setVSync(true);

        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height){
            windowData& data = *(windowData*)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            WindowResizeEvent event(width, height);
            data.eventCallback(event);
        });

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window){
           windowData& data = *(windowData*) glfwGetWindowUserPointer(window);
           WindowCloseEvent event;
           data.eventCallback(event);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
            windowData& data = *(windowData*) glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS:{
                    KeyPressedEvent event(key, 0);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:{
                    KeyReleasedEvent event(key);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_REPEAT:{
                    KeyPressedEvent event(key, 1);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character){
            windowData& data = *(windowData*) glfwGetWindowUserPointer(window);
            KeyTypeEvent event(character);
            data.eventCallback(event);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods){
            windowData& data = *(windowData*) glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS:{
                    MouseButtonPressedEvent event(button);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:{
                    MouseButtonReleaseEvent event(button);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset){
            windowData& data = *(windowData*) glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.eventCallback(event);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xPos, double yPos){
            windowData& data = *(windowData*) glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.eventCallback(event);
        });


    }

    void WindowsWindow::shutdown() {
        glfwDestroyWindow(window);
    }

    void WindowsWindow::onUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    void WindowsWindow::setVSync(bool enabled) {
        if(enabled){
            glfwSwapInterval(1);
        } else{
            glfwSwapInterval(0);
        }

        data_window.verticalSync = enabled;
    }

    bool WindowsWindow::isVSync() const {
        return data_window.verticalSync;
    }

} // yic