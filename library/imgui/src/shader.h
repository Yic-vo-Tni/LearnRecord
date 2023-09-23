//
// Created by lenovo on 9/28/2023.
//

#ifndef LEARN_RECORD_SHADER_H
#define LEARN_RECORD_SHADER_H

#include "glad/glad.h"
//#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"

#include "string"
#include "fstream"
#include "sstream"
#include "iostream"

namespace yic{
    class Shader{
    public:
        Shader(const char* vertex_path, const char* fragment_path);
        ~Shader() = default;

        Shader(const Shader&) = delete;
        Shader &operator = (const Shader&) = delete;

        unsigned int ID;

        void use();
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;

    private:
        void checkCompileErrors(unsigned int shader, std::string type);



    };
}

#endif //LEARN_RECORD_SHADER_H
