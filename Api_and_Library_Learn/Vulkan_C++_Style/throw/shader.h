//
// Created by lenovo on 10/16/2023.
//

#ifndef LEARN_RECORD_SHADER_H
#define LEARN_RECORD_SHADER_H

#include "vulkan/vulkan.hpp"
#include "../src/device.h"

namespace yic {

    class Shader {
    public:
        Shader(const std::string& vertexSource, const std::string& fragmentSource, Device& device);
        ~Shader();

        std::vector<vk::PipelineShaderStageCreateInfo> shaderStage() { return shaderStage_;};

    private:
        void createShaderModule(const std::string& vertexSource, const std::string& fragmentSource);
        std::string readFile(const std::string& fileName);
        void createShaderStage();

        vk::ShaderModule vertexModule;
        vk::ShaderModule fragmentModule;
        std::vector<vk::PipelineShaderStageCreateInfo> shaderStage_;

        Device& device;

    };

} // yic

#endif //LEARN_RECORD_SHADER_H
