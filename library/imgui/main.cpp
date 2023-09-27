//
// Created by lenovo on 9/27/2023.
//
#if 0
#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "yic_define.h"

void process_input(GLFWwindow* window);


int main(){
    GLFWwindow* window;
    const int width = 1200;
    const int height = 800;
    std::string text{"Text"};
    char textbox[50]{"text box"};
    ImVec4 color{};

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "yic", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        std::cout << "failed to initialize glad" << std::endl;
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext(nullptr);
    ImGuiIO& io = ImGui::GetIO();
    (void)io;

    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    while (!glfwWindowShouldClose(window)){
        process_input(window);

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Yicvot", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_MenuBar);
        ImGui::Text(text.c_str());

        if (ImGui::Button("Button")){
            std::cout << "button" << std::endl;
            text = "you click the button";
        }

        ImGui::InputText("Test box", textbox, IM_ARRAYSIZE(textbox));
        //get the size of text

#if LIST_BOX
        ImGui::BeginListBox("list box");
        for(size_t i = 0; i < 10; i++){
            if(ImGui::Selectable(std::to_string(i).c_str()), std::to_string(i) != text){
                text = std::to_string(i);
                //to_string : 将数字常量转换为string
            }
        }
        ImGui::EndListBox();
#endif

#if COMBO
        if (ImGui::BeginCombo("yic", text.c_str())){
            for(size_t i = 0; i < 100; i++){
                if(ImGui::Selectable(std::to_string(i).c_str())){
                    text = i;
                }
            }

            ImGui::EndCombo();
        }

        ImGui::ColorEdit4("text color", (float*)&color, ImGuiColorEditFlags_::ImGuiColorEditFlags_HDR);
#endif



        ImGui::End();


        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    return 0;
}

void process_input(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}
#endif

#include "application.h"

int main(){
    yic::Application application{};

    try {
        application.run();
    } catch (const std::exception& e){
        std::cout << e.what() << std::endl;
        return -1;
    }


    return 0;
}
