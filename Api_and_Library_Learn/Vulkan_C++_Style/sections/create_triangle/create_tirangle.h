//
// Created by lenovo on 10/19/2023.
//

#ifndef LEARN_RECORD_CREATE_TIRANGLE_H
#define LEARN_RECORD_CREATE_TIRANGLE_H

//
// Created by lenovo on 10/15/2023.
//

#ifndef LEARN_RECORD_DEVICE_H
#define LEARN_RECORD_DEVICE_H

#include <iostream>
#include <optional>
#include <set>
#include <limits>
#include <fstream>

#include "window.h"

namespace yic {

    class Device {
    public:
        struct QueueFamilyIndices{
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool isComplete(){
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        struct SwapChainSupportDetails{
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> formats;
            std::vector<vk::PresentModeKHR> presentModes;
        };

        Device(Window& window);
        ~Device();

        void drawFrame();

        auto& device_(){ return device; }

    private:
        void init();
        void clean();

        vk::Instance instance;
        vk::DebugUtilsMessengerEXT debugMessenger;
        vk::SurfaceKHR surface;

        vk::PhysicalDevice physicalDevice;
        vk::Device device;

        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

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


        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicDevice();
        void createSwapChain();
        void createImageViews();
        void createRenderPass();
        void createGraphicsPipeline();
        void createFrameBuffers();
        void createCommandPool();
        void createCommandBuffers();
        void createSyncObjects();

        bool checkValidationLayerSupport();
        void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo);
        std::vector<const char*> getRequiredExtensions();

        QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice& device);

        vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
        vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
        SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device);

        vk::ShaderModule createShaderModule(const std::vector<char>& code);
        static std::vector<char> readFile(const std::string& fileName);

        void recordCommandBuffer(vk::CommandBuffer& commandBuffer, uint32_t imageIndex);
    };

}
#endif //LEARN_RECORD_DEVICE_H


#endif //LEARN_RECORD_CREATE_TIRANGLE_H
