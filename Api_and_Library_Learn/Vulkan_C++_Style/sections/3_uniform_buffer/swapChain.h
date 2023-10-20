//
// Created by lenovo on 10/19/2023.
//

#ifndef LEARN_RECORD_SWAPCHAIN_H
#define LEARN_RECORD_SWAPCHAIN_H

#include "device.h"
#include "limits"

namespace yic{
    class SwapChain{
    public:
        struct SwapChainSupportDetails{
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> formats;
            std::vector<vk::PresentModeKHR> presentModes;
        };

        SwapChain(Window& window, Device& device);
        ~SwapChain();

        auto& swapChain_() { return swapChain; }
        auto& swapChainImageFormat_() { return swapChainImageFormat; }
        auto& swapChainImageViews_() { return swapChainImageViews; }
        auto& swapChainExtent_() { return swapChainExtent; }

    private:
        Window& window_;
        Device& device_;

        vk::SwapchainKHR swapChain;
        vk::Format swapChainImageFormat;
        vk::Extent2D swapChainExtent;
        std::vector<vk::Image> swapChainImages;
        std::vector<vk::ImageView> swapChainImageViews;

        void createSwapChain();
        void createImageViews();

        vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
        vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities);
        SwapChainSupportDetails querySwapChainSupport(const vk::PhysicalDevice& device);

    };
}

#endif //LEARN_RECORD_SWAPCHAIN_H
