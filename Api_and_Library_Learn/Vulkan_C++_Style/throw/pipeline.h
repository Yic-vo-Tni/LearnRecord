//
// Created by lenovo on 10/16/2023.
//

#ifndef LEARN_RECORD_PIPELINE_H
#define LEARN_RECORD_PIPELINE_H

#include "vulkan/vulkan.hpp"
#include "shader.h"
#include "../src/device.h"
#include "swap_chain.h"
#include "memory"

namespace yic {

    class Pipeline {
    public:
        struct pipelineConfig{
            vk::PipelineVertexInputStateCreateInfo inputState;
            vk::PipelineInputAssemblyStateCreateInfo inputAssembly;

            vk::PipelineViewportStateCreateInfo viewportState;

            vk::PipelineRasterizationStateCreateInfo rasterization;
            vk::PipelineMultisampleStateCreateInfo multisample;

            vk::PipelineColorBlendStateCreateInfo colorBlend;
            vk::PipelineColorBlendAttachmentState colorBlendAttach;
        }configInfo;

        Pipeline(int w, int h, Shader& shader, Device& device, SwapChain& swapChain);
        ~Pipeline();

    private:
        void createLayout();
        void createRenderPass();
        void createPipeline(int w, int h);


        vk::Pipeline pipeline;
        vk::PipelineLayout layout;
        vk::RenderPass renderPass;
        Shader& shader;
        Device& device_;
        SwapChain& swapChain_;
    };

} // yic

#endif //LEARN_RECORD_PIPELINE_H
