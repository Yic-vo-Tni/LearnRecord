//
// Created by lenovo on 9/28/2023.
//

#ifndef LEARN_RECORD_RENDER_H
#define LEARN_RECORD_RENDER_H

#include "glad/glad.h"
#include "shader.h"
//#include "camera.h"
#include "window.h"
#include "camera.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "unistd.h"



namespace yic{


    class Render{
    public:

        Render(int w, int h, Window window);
        ~Render();

        Render(const Render&) = delete;
        Render &operator = (const Render&) = delete;



        void setBuffer();
        //static void loadTexture();
        void render(float w, float h);
        void loadTexture();




    private:
        Window window;

        unsigned int VBO, VAO, EBO{};
        float width{}, height{};
        unsigned int texture1, texture2;
        glm::mat4 view{};

        char buf[256];

        Shader shader{R"(F:\Program\LearningRecord\library\imgui\src\shader\shader.vert)", R"(F:\Program\LearningRecord\library\imgui\src\shader\shader.frag)"};
        //Shader shader{"../src/shader/shader.vert", "../src/shader/shader.frag"};
    };
}

#endif //LEARN_RECORD_RENDER_H
