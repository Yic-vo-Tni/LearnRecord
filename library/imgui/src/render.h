//
// Created by lenovo on 9/28/2023.
//

#ifndef LEARN_RECORD_RENDER_H
#define LEARN_RECORD_RENDER_H

#include "shader.h"
#include "camera.h"

namespace yic{


    class Render{
    public:
        Render() = default;
        ~Render();

        Render(const Render&) = delete;
        Render &operator = (const Render&) = delete;

        void setBuffer();


    private:
        unsigned int VBO, VAO;

        Camera camera{glm::vec3(0.0f, 0.0f, 3.0f)};
        Shader shader{R"(F:\Program\LearningRecord\library\imgui\src\shader\shader.vert)", R"(F:\Program\LearningRecord\library\imgui\src\shader\shader.frag)"};
    };
}

#endif //LEARN_RECORD_RENDER_H
