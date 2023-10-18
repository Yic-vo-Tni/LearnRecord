//
// Created by lenovo on 10/14/2023.
//

#ifndef LEARN_RECORD_SWAP_CHAIN_H
#define LEARN_RECORD_SWAP_CHAIN_H

#include "../src/device.h"

namespace yic{

    class SwapChain final {
    public:
        struct swapChainInfo{
            vk::Extent2D imageExtent;
            uint32_t imageCount;
            vk::SurfaceFormatKHR surfaceFormat;
            vk::SurfaceTransformFlagsKHR transform;
            vk::PresentModeKHR presentMode;
        } info;

        struct swapChainSupportDetails{
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> formats;
            std::vector<vk::PresentModeKHR> presentModes;
        };

        SwapChain(int w, int h, Device& device, Window& window);
        ~SwapChain();

        SwapChain(const SwapChain&) = delete;
        SwapChain& operator = (const SwapChain&) = delete;



    private:
        void queryInfo(int w, int h);
        void createSwapChain();
        void getImages();
        void createImageViews();
        swapChainSupportDetails querySwapChainSupport();
        vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
        vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes);
        vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

        vk::SwapchainKHR swapChain_;
        std::vector<vk::Image> images;
        std::vector<vk::ImageView> imageViews;
        vk::Format swapChainImageFormat;
        vk::Extent2D swapChainExtent;

        Window& window_;
        Device& device_;

    };

} // yic

#endif //LEARN_RECORD_SWAP_CHAIN_H
