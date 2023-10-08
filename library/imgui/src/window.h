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
    static Camera camera(glm::vec3(0.0f, 0.0f, 1.0f));
    static bool firstMouse = true;
    static bool mouse = true;

    inline struct position {
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
        float pitch =  0.0f;
        float lastX =  800.0f / 2.0;
        float lastY =  600.0 / 2.0;
        float fov   =  45.0f;
    }position;

    class Window {
    public:

        Window(int w, int h, std::string name);
        ~Window() = default;



        float deltaTime = 0.0f;	// time between current frame and last frame
        float lastFrame = 0.0f;

        GLFWwindow* getWindow(){return window;}
        bool windowShouldClose(){ return glfwWindowShouldClose(window);}

        void processInput(GLFWwindow* window);
        void imguiDraw();

    private:
        void initWindow();
        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn);
        static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

        GLFWwindow* window;
        std::string name{};
        int width{};
        int height{};
        //position position;



        std::string text{"Text"};
        char textbox[50]{"text box"};
        ImVec4 color{};
    };

}

#endif //LEARN_RECORD_WINDOW_H
