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

    inline const LoadVertex::uniformColor uniformColor = {0.f, 1.f, 0.f};

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

        vk::CommandPool commandPool;
        std::vector<vk::CommandBuffer> commandBuffers;
        std::vector<vk::Framebuffer> swapChainFrameBuffers;

        std::vector<vk::Semaphore> imageAvailableSemaphores;
        std::vector<vk::Semaphore> renderFinishedSemaphores;
        std::vector<vk::Fence> inFlightFences;
        uint32_t currentFrame = 0;

        bool frameBufferResized = false;

        vk::DescriptorPool descriptorPool;
        std::vector<vk::DescriptorSet> desSet;

        void createVertexBuffer();
        void bufferData();
        void createUniformBuffer();
        void createUniformBufferData();
        void copyBuffer(vk::Buffer& src, vk::Buffer& dst, size_t size, size_t srcOffset, size_t dstOffset);
        inline void freeCommandBuffer(vk::CommandBuffer);
        void createDescriptorPool();
        void allocateSets();
        void updateSets();


        void recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
        vk::CommandBufferAllocateInfo commandBufferInfo();

        std::unique_ptr<LoadVertex> hostVertexBuffer;
        std::unique_ptr<LoadVertex> deviceVertexBuffer;
        std::vector<std::unique_ptr<LoadVertex>> hostUniformBuffer;
        std::vector<std::unique_ptr<LoadVertex>> deviceUniformBuffer;
    };

}



#endif //LEARN_RECORD_CREATE_TIRANGLE_H
