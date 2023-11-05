//
// Created by lenovo on 10/19/2023.
//

#ifndef LEARN_RECORD_PIPELINE_H
#define LEARN_RECORD_PIPELINE_H

#include "swapChain.h"
#include "fstream"
#include "loadVertex.h"
#include "memory"

namespace yic{
    class Pipeline{
    public:
        Pipeline(Device& device);
        ~Pipeline();

        auto& renderPass_() { return renderPass; }
        auto& graphicsPipeline_() { return graphicsPipeline; }
        auto& desSetLayout_() { return desSetLayout; }
        auto& pipelineLayout_() { return pipelineLayout; }
        auto& yic_swapchain() { return swapChain_; }

    private:
        Device& device_;
//        SwapChain& swapChain_;
        std::unique_ptr<SwapChain> swapChain_ = std::make_unique<SwapChain>(device_.window(), device_);

        vk::RenderPass renderPass;
        vk::PipelineLayout pipelineLayout;
        vk::Pipeline graphicsPipeline;
        vk::DescriptorSetLayout desSetLayout;

        void createRenderPass();
        void createGraphicsPipeline();

        vk::ShaderModule createShaderModule(const std::vector<char>& code);
        static std::vector<char> readFile(const std::string& fileName);
        vk::DescriptorSetLayout createSetLayout();
    };
}

#endif //LEARN_RECORD_PIPELINE_H
