//
// Created by lenovo on 9/28/2023.
//

#ifndef LEARN_RECORD_RENDER_H
#define LEARN_RECORD_RENDER_H

#include "glad/glad.h"
#include "shader.h"
//#include "camera.h"
#include "window.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb-master/stb_image.h"

namespace yic{


    class Render{
    public:

        //Render();
        Render();
        ~Render();

        Render(const Render&) = delete;
        Render &operator = (const Render&) = delete;



        void setBuffer();
        //static void loadTexture();
        void render();
        void loadTexture();



    private:
        unsigned int VBO, VAO, EBO{};
        float width{};
        float height{};
        float deltaTime = 0.0f;	// time between current frame and last frame
        float lastFrame = 0.0f;
        unsigned int texture1, texture2;




        Shader shader{R"(F:\Program\LearningRecord\library\imgui\src\shader\shader.vert)", R"(F:\Program\LearningRecord\library\imgui\src\shader\shader.frag)"};
    };
}

#endif //LEARN_RECORD_RENDER_H
