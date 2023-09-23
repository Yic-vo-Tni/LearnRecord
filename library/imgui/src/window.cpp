//
// Created by lenovo on 9/28/2023.
//

#include "window.h"

#include <utility>

namespace yic{
    Window::Window(int w, int h, std::string name) : width(w), height(h), name(std::move(name)) {
        initWindow();
    }

    void Window::initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

        //glfwSetCursorPosCallback(window, reinterpret_cast<GLFWcursorposfun>(mouseCallback));
        //glfwSetScrollCallback(window, scrollCallback);

        //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
            std::cout << "failed to initialize glad" << std::endl;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext(nullptr);
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

    void Window::imguiDraw() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Yicvot", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
        ImGui::Text(text.c_str());

        ImGui::End();


        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Window::processInput(GLFWwindow *window) {
        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
            glfwSetWindowShouldClose(window, true);
        }
//        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//            camera.ProcessKeyboard(FORWARD, deltaTime);
//        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//            camera.ProcessKeyboard(BACKWARD, deltaTime);
//        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//            camera.ProcessKeyboard(LEFT, deltaTime);
//        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//            camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Window::mouseCallback(GLFWwindow *window, double xposIn, double yposIn, int width, int height) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        float lastX = width / 2.0f;
        float lastY = height / 2.0f;
        bool firstMouse = true;
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }

    void Window::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }


}


