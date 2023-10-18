//
// Created by lenovo on 10/16/2023.
//

#include <fstream>
#include "shader.h"

namespace yic {

    Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource, Device& device) : device(device) {
        createShaderModule(readFile(vertexSource), readFile(fragmentSource));
        createShaderStage();
    }

    Shader::~Shader() {
        device.getDevice().destroyShaderModule(vertexModule);
        device.getDevice().destroyShaderModule(fragmentModule);
    }

    void Shader::createShaderModule(const std::string& vertexSource, const std::string& fragmentSource) {
        vk::ShaderModuleCreateInfo createInfo;
        createInfo.codeSize = vertexSource.size();
        createInfo.pCode = (uint32_t*)vertexSource.data();

        vertexModule = device.getDevice().createShaderModule(createInfo);

        createInfo.codeSize = fragmentSource.size();
        createInfo.pCode = (uint32_t*)fragmentSource.data();
        fragmentModule = device.getDevice().createShaderModule(createInfo);
    }

    std::string Shader::readFile(const std::string &fileName) {
        std::ifstream file(fileName, std::ios::binary | std::ios::ate);

        if(!file.is_open()){
            std::cout << "read" << fileName << "failed" << std::endl;
            return std::string{};
        }

        auto size = file.tellg();
        std::string context;
        context.resize(size);
        file.seekg(0);

        file.read(context.data(), context.size());

        return context;
    }

    void Shader::createShaderStage() {
        shaderStage_.resize(2);
        shaderStage_[0].setStage(vk::ShaderStageFlagBits::eVertex)
                .setPName("main")
                .setModule(vertexModule);
        shaderStage_[1].setStage(vk::ShaderStageFlagBits::eFragment)
                .setPName("main")
                .setModule(fragmentModule);
    }


} // yic