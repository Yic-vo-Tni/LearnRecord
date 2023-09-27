//
// Created by lenovo on 9/28/2023.
//

#include "window.h"

namespace yic{
    Window::Window(int w, int h, std::string name) : width(w), height(h), name(name) {
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
    }

    void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }
}
