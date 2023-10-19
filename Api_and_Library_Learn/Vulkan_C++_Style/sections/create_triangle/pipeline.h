//
// Created by lenovo on 10/19/2023.
//

#ifndef LEARN_RECORD_PIPELINE_H
#define LEARN_RECORD_PIPELINE_H

#include "swapChain.h"
#include "fstream"

namespace yic{
    class Pipeline{
    public:
        Pipeline(Device& device, SwapChain& swapChain);
        ~Pipeline();

        auto& renderPass_() { return renderPass; }
        auto& graphicsPipeline_() { return graphicsPipeline; }

    private:
        Device& device_;
        SwapChain& swapChain_;

        vk::RenderPass renderPass;
        vk::PipelineLayout pipelineLayout;
        vk::Pipeline graphicsPipeline;

        void createRenderPass();
        void createGraphicsPipeline();

        vk::ShaderModule createShaderModule(const std::vector<char>& code);
        static std::vector<char> readFile(const std::string& fileName);
    };
}

#endif //LEARN_RECORD_PIPELINE_H
