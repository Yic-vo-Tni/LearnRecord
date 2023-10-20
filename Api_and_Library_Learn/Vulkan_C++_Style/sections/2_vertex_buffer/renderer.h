//
// Created by lenovo on 10/19/2023.
//

#ifndef LEARN_RECORD_CREATE_TIRANGLE_H
#define LEARN_RECORD_CREATE_TIRANGLE_H

#include "pipeline.h"

namespace yic {
    inline const std::array<LoadVertex::Vertex, 3> vertices = {
            LoadVertex::Vertex{ 0.0f, -0.8f},
            LoadVertex::Vertex{ 0.8f,  0.8f},
            LoadVertex::Vertex{-0.8f,  0.8f}
    };

    class Renderer {
    public:
        Renderer(Device& device, SwapChain& swapChain, Pipeline& pipeline);
        ~Renderer();

        void drawFrame();

    private:
        Device& device_;
        SwapChain& swapChain_;
        Pipeline& pipeline_;

        void createFrameBuffers();
        void createCommandPool();
        void createCommandBuffers();
        void createSyncObjects();
        void createVertexBuffer();
        void bufferData();

        vk::CommandPool commandPool;
        std::vector<vk::CommandBuffer> commandBuffers;
        std::vector<vk::Framebuffer> swapChainFrameBuffers;

        std::vector<vk::Semaphore> imageAvailableSemaphores;
        std::vector<vk::Semaphore> renderFinishedSemaphores;
        std::vector<vk::Fence> inFlightFences;
        uint32_t currentFrame = 0;

        bool frameBufferResized = false;

        void recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
        vk::CommandBufferAllocateInfo commandBufferInfo();
        inline void freeCommandBuffer(vk::CommandBuffer);
        std::unique_ptr<LoadVertex> hostVertexBuffer;
        std::unique_ptr<LoadVertex> deviceVertexBuffer;
    };

}



#endif //LEARN_RECORD_CREATE_TIRANGLE_H
