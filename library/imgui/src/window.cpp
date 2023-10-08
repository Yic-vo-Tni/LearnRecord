//
// Created by lenovo on 9/28/2023.
//

#include "window.h"

#include <utility>

namespace yic {
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
        glfwSetScrollCallback(window, scrollCallback);

//        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
//            glfwSetCursorPosCallback(window,mouseCallback);
//            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//        }

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            std::cout << "failed to initialize glad" << std::endl;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext(nullptr);
        ImGuiIO &io = ImGui::GetIO();
        (void) io;

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
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
            mouse = false;
            glfwSetCursorPosCallback(window,mouseCallback);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }


        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
            mouse = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

//        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//            camera.ProcessKeyboard(Forward, deltaTime);
//        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//            camera.ProcessKeyboard(Backward, deltaTime);
//        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//            camera.ProcessKeyboard(Left, deltaTime);
//        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//            camera.ProcessKeyboard(Right_, deltaTime);

        float cameraSpeed = static_cast<float>(2.5 * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            position.cameraPos += cameraSpeed * position.cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            position.cameraPos -= cameraSpeed * position.cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            position.cameraPos -= glm::normalize(glm::cross(position.cameraFront, position.cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            position.cameraPos += glm::normalize(glm::cross(position.cameraFront, position.cameraUp)) * cameraSpeed;
    }

    void Window::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Window::mouseCallback(GLFWwindow *window, double xposIn, double yposIn) {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if(mouse){
            return;
        }

        if (firstMouse)
        {
            position.lastX = xpos;
            position.lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - position.lastX;
        float yoffset = position.lastY - ypos; // reversed since y-coordinates go from bottom to top
        position.lastX = xpos;
        position.lastY = ypos;

        float sensitivity = 0.1f; // change this value to your liking
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        position.yaw += xoffset;
        position.pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (position.pitch > 89.0f)
            position.pitch = 89.0f;
        if (position.pitch < -89.0f)
            position.pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(position.yaw)) * cos(glm::radians(position.pitch));
        front.y = sin(glm::radians(position.pitch));
        front.z = sin(glm::radians(position.yaw)) * cos(glm::radians(position.pitch));
        position.cameraFront = glm::normalize(front);
    }

    void Window::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
        camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }


}


