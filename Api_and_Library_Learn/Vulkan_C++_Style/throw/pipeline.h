//
// Created by lenovo on 10/19/2023.
//

#ifndef LEARN_RECORD_CREATE_TIRANGLE_H
#define LEARN_RECORD_CREATE_TIRANGLE_H

#include <limits>
#include <fstream>

#include "swapChain.h"

namespace yic {

    class Renderer {
    public:
        Renderer(Window& window, Device& device, SwapChain& swapChain);
        ~Renderer();


        void drawFrame();

    private:
        void init();
        void clean();

        std::vector<vk::Framebuffer> swapChainFrameBuffers;

        vk::RenderPass renderPass;
        vk::PipelineLayout pipelineLayout;
        vk::Pipeline graphicsPipeline;

        vk::CommandPool commandPool;
        std::vector<vk::CommandBuffer> commandBuffers;

        std::vector<vk::Semaphore> imageAvailableSemaphores;
        std::vector<vk::Semaphore> renderFinishedSemaphores;
        std::vector<vk::Fence> inFlightFences;
        uint32_t currentFrame = 0;

        bool frameBufferResized = false;

        Window& window_;
        Device& device_;
        SwapChain& swapChain_;

        void createRenderPass();
        void createGraphicsPipeline();
        void createFrameBuffers();
        void createCommandPool();
        void createCommandBuffers();
        void createSyncObjects();


        vk::ShaderModule createShaderModule(const std::vector<char>& code);
        static std::vector<char> readFile(const std::string& fileName);

        void recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
    };

}



#endif //LEARN_RECORD_CREATE_TIRANGLE_H
