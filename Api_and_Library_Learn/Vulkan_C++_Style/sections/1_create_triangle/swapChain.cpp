//
// Created by lenovo on 10/19/2023.
//

#include "swapChain.h"

namespace yic{
    SwapChain::SwapChain(Window &window, Device &device) : window_{window}, device_{device}{
        createSwapChain();
        createImageViews();
    }

    SwapChain::~SwapChain() {
        for(auto& swapChainImageView : swapChainImageViews) { device_.device_().destroy(swapChainImageView); }
        device_.device_().destroySwapchainKHR(swapChain);
    }


    void SwapChain::createSwapChain() {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device_.physicalDevice_());

        vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        vk::Extent2D extent2D = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount){
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR createInfo{};
        createInfo.setSurface(device_.surface_())
                .setMinImageCount(imageCount)
                .setImageFormat(surfaceFormat.format)
                .setImageColorSpace(surfaceFormat.colorSpace)
                .setImageExtent(extent2D)
                .setImageArrayLayers(1)
                .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

        Device::QueueFamilyIndices indices = device_.findQueueFamilies(device_.physicalDevice_());
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if(indices.graphicsFamily != indices.presentFamily){
            createInfo.setImageSharingMode(vk::SharingMode::eConcurrent)
                    .setQueueFamilyIndexCount(2)
                    .setPQueueFamilyIndices(queueFamilyIndices);
        } else{
            createInfo.setImageSharingMode(vk::SharingMode::eExclusive)
                    .setQueueFamilyIndexCount(0)
                    .setPQueueFamilyIndices(0);
        }

        createInfo.setPreTransform(swapChainSupport.capabilities.currentTransform)
                .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
                .setPresentMode(presentMode)
                .setClipped(VK_TRUE);

        createInfo.setOldSwapchain(VK_NULL_HANDLE);

        swapChain = device_.device_().createSwapchainKHR(createInfo);
        swapChainImages = device_.device_().getSwapchainImagesKHR(swapChain);
        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent2D;
    }

    void SwapChain::createImageViews() {
        swapChainImageViews.resize(swapChainImages.size());

        vk::ComponentMapping components;
        components.setA(vk::ComponentSwizzle::eIdentity)
                .setB(vk::ComponentSwizzle::eIdentity)
                .setG(vk::ComponentSwizzle::eIdentity)
                .setR(vk::ComponentSwizzle::eIdentity);
        vk::ImageSubresourceRange subresourceRange;
        subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor)  // 设置图像的使用目的（颜色）
                .setBaseMipLevel(0)  // 设置mip级别的基本级别（从0级开始）
                .setLevelCount(1)     // 设置mip级别的层数（本例中为1，表示只使用一个mip级别）
                .setBaseArrayLayer(0) // 设置数组层的基本层（从0层开始）
                .setLayerCount(1);     // 设置数组层的层数（本例中为1，表示只使用一个数组层）

        for(int i = 0; i < swapChainImages.size(); i++){
            vk::ImageViewCreateInfo createInfo{};

            createInfo.setImage(swapChainImages[i])
                    .setViewType(vk::ImageViewType::e2D)
                    .setFormat(swapChainImageFormat)
                    .setComponents(components)
                    .setSubresourceRange(subresourceRange);

            swapChainImageViews[i] = device_.device_().createImageView(createInfo);
        }
    }

    SwapChain::SwapChainSupportDetails SwapChain::querySwapChainSupport(const vk::PhysicalDevice &device) {
        SwapChainSupportDetails details{};

        details.capabilities = device.getSurfaceCapabilitiesKHR(device_.surface_());
        details.formats = device.getSurfaceFormatsKHR(device_.surface_());
        details.presentModes = device.getSurfacePresentModesKHR(device_.surface_());

        return details;
    }


    vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
        for(const auto& available : availableFormats){
            if(available.format == vk::Format::eR8G8B8A8Srgb && available.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
                return available;
            }
        }

        return availableFormats[0];
    }

    vk::PresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
        for(const auto& available : availablePresentModes){
            if(available == vk::PresentModeKHR::eMailbox){
                return available;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D SwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) {
        if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
            return capabilities.currentExtent;
        } else{
            int width, height;
            glfwGetFramebufferSize(window_.getWindow(), &width, &height);

            vk::Extent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.width, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }


}