//
// Created by lenovo on 10/19/2023.
//

#ifndef LEARN_RECORD_CREATE_TIRANGLE_H
#define LEARN_RECORD_CREATE_TIRANGLE_H

#include "pipeline.h"

namespace yic {

    class Renderer {
    public:
        Renderer(Device& device, SwapChain& swapChain, Pipeline& pipeline);
        ~Renderer();


        void drawFrame();

    private:
        void init();
        void clean();

        std::vector<vk::Framebuffer> swapChainFrameBuffers;



        vk::CommandPool commandPool;
        std::vector<vk::CommandBuffer> commandBuffers;

        std::vector<vk::Semaphore> imageAvailableSemaphores;
        std::vector<vk::Semaphore> renderFinishedSemaphores;
        std::vector<vk::Fence> inFlightFences;
        uint32_t currentFrame = 0;

        bool frameBufferResized = false;

        Device& device_;
        SwapChain& swapChain_;
        Pipeline& pipeline_;


        void createFrameBuffers();
        void createCommandPool();
        void createCommandBuffers();
        void createSyncObjects();




        void recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
    };

}



#endif //LEARN_RECORD_CREATE_TIRANGLE_H
