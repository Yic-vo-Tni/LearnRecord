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


        struct SwapChainSupportDetails{
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> formats;
            std::vector<vk::PresentModeKHR> presentModes;
        };

        Renderer(Window& window, Device& device);
        ~Renderer();


        void drawFrame();

    private:
        void init();
        void clean();



        vk::SwapchainKHR swapChain;
        vk::Format swapChainImageFormat;
        vk::Extent2D swapChainExtent;
        std::vector<vk::Image> swapChainImages;
        std::vector<vk::ImageView> swapChainImageViews;
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

        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createGraphicsPipeline();
        void createFrameBuffers();
        void createCommandPool();
        void createCommandBuffers();
        void createSyncObjects();

        vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
        vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
        SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device);

        vk::ShaderModule createShaderModule(const std::vector<char>& code);
        static std::vector<char> readFile(const std::string& fileName);

        void recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
    };

}



#endif //LEARN_RECORD_CREATE_TIRANGLE_H
