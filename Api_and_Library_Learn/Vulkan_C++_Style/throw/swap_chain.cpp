//
// Created by lenovo on 10/14/2023.
//

#include "swap_chain.h"
#include "limits"

namespace yic{

    SwapChain::SwapChain(int w, int h, yic::Device &device, yic::Window &window) : device_(device), window_(window) {
        //queryInfo(w, h);

        if (device_.getDevice() == nullptr){
            std::cout << "device is nullptr" << std::endl;
        }
        createSwapChain();
        std::cout << "create" << std::endl;
//        getImages();
//        createImageViews();

    }

    SwapChain::~SwapChain() {
//        for(auto& imageView : imageViews){
//            device_.getDevice().destroyImageView(imageView);
//        }
//
        device_.getDevice().destroySwapchainKHR(swapChain_);
    }

    void SwapChain::createSwapChain() {
        std::cout << "??" << std::endl;
        if(device_.getDevice() == nullptr){
            std::cout << "null" << std::endl;
        } else{
            std::cout << device_.getDevice() << std::endl;
        }
        auto& surface = device_.getSurface();
        auto& queueFamily = device_.getQueueFamilyIndices();
        auto& device = device_.getDevice();
        swapChainSupportDetails swapChain = querySwapChainSupport();

        vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChain.formats);
        vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChain.presentModes);
        vk::Extent2D extent2D = chooseSwapExtent(swapChain.capabilities, window_.getWindow());

        uint32_t imageCount = swapChain.capabilities.minImageCount + 1;
        if(swapChain.capabilities.maxImageCount > 0 && imageCount > swapChain.capabilities.maxImageCount){
            imageCount = swapChain.capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR createInfo{};
        createInfo.setSurface(device_.getSurface())
                .setMinImageCount(imageCount)
                .setImageColorSpace(surfaceFormat.colorSpace)
                .setImageExtent(extent2D)
                .setImageArrayLayers(1)
                .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

        auto& indices = device_.getQueueFamilyIndices();
        uint32_t queueFamilyIndices[] = {indices.graphicsQueue.value(), indices.presentQueue.value()};

        std::cout << indices.graphicsQueue.value() << std::endl;
        std::cout << indices.presentQueue.value() << std::endl;

        if(indices.graphicsQueue != indices.presentQueue){
            createInfo.setImageSharingMode(vk::SharingMode::eConcurrent)
                    .setQueueFamilyIndexCount(2)
                    .setPQueueFamilyIndices(queueFamilyIndices);
        } else{
            createInfo.setImageSharingMode(vk::SharingMode::eExclusive);
        }

        createInfo.setPreTransform(swapChain.capabilities.currentTransform)
                .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
                .setPresentMode(presentMode)
                .setClipped(vk::True)
                .setOldSwapchain(VK_NULL_HANDLE);

        std::cout << "success" << std::endl;
        swapChain_ = device_.getDevice().createSwapchainKHR(createInfo);
        std::cout << "oK success" << std::endl;
        //std::vector<vk::Image> swapChainImages = device_.getDevice().getSwapchainImagesKHR(swapChain_);



//        vk::SwapchainCreateInfoKHR createInfo;
//
//        createInfo.setClipped(true)
//                .setImageArrayLayers(1)
//                .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
//                .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
//                .setSurface(device_.getSurface())
//                .setImageColorSpace(info.surfaceFormat.colorSpace)
//                .setImageFormat(info.surfaceFormat.format)
//                .setImageExtent(info.imageExtent)
//                .setMinImageCount(info.imageCount)
//                .setPresentMode(info.presentMode);
//
//        auto& queueIndices = device_.getQueueFamilyIndices();
//        if(queueIndices.graphicsQueue.value() == queueIndices.presentQueue.value()){
//            createInfo.setQueueFamilyIndices(queueIndices.graphicsQueue.value())
//                    .setImageSharingMode(vk::SharingMode::eExclusive);
//        } else {
//            std::array indices = {queueIndices.graphicsQueue.value(), queueIndices.presentQueue.value()};
//            createInfo.setQueueFamilyIndices(indices)
//                    .setImageSharingMode(vk::SharingMode::eConcurrent);
//        }
//
//        swapChain = device_.getDevice().createSwapchainKHR(createInfo);
//        if (swapChain){
//            std::cout << "swapChain is nullptr" << std::endl;
//        }
    }

    vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats){
        for(const auto& available : availableFormats){
            if(available.format == vk::Format::eR8G8B8A8Srgb && available.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
                return available;
            }
        }
        return availableFormats[0];
    }

    vk::PresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availablePresentModes){
        for(const auto& available : availablePresentModes){
            if(available == vk::PresentModeKHR::eMailbox){
                return available;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D SwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities, GLFWwindow* window){
        if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
            return capabilities.currentExtent;
        } else{
            int width, height;
            glfwGetFramebufferSize(window, &width, &height);

            vk::Extent2D actualExtent = {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.height);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }

    SwapChain::swapChainSupportDetails SwapChain::querySwapChainSupport() {
        swapChainSupportDetails details;
        auto& phy_device = device_.getPhysicalDevice();
        auto& surface = device_.getSurface();
        details.capabilities = phy_device.getSurfaceCapabilitiesKHR(surface);
        details.formats = phy_device.getSurfaceFormatsKHR(surface);
        details.presentModes = phy_device.getSurfacePresentModesKHR(surface);
        return details;
    }

    void SwapChain::queryInfo(int w, int h) {
        auto& phyDevice = device_.getPhysicalDevice();
        auto& surface = device_.getSurface();
        auto formats = phyDevice.getSurfaceFormatsKHR(surface);
        info.surfaceFormat = formats[0];
        for(const auto& format : formats){
            if(format.format == vk::Format::eR8G8B8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
                info.surfaceFormat = format;
                break;
            }
        }

        auto capabilities = phyDevice.getSurfaceCapabilitiesKHR(surface);
        info.imageCount = std::clamp<uint32_t>(2, capabilities.minImageCount, capabilities.maxImageCount);

        info.imageExtent.width = std::clamp<uint32_t>(w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        info.imageExtent.height = std::clamp<uint32_t>(h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        info.transform = capabilities.currentTransform;

        auto presents = phyDevice.getSurfacePresentModesKHR(surface);
        info.presentMode = vk::PresentModeKHR::eFifo;
        for(const auto& present : presents) {
            if (present == vk::PresentModeKHR::eMailbox) {
                info.presentMode = present;
                std::cout << "Present Mode:  eMailbox" << std::endl;
                break;
            }
        }
    }

//    void SwapChain::queryInfo(int w, int h) {
//        auto physicalDevice = device_.getPhysicalDevice();
//        auto surface = device_.getSurface();
//        auto formats = physicalDevice.getSurfaceFormatsKHR(surface);
//        info.surfaceFormat = formats[0];
//        for(const auto& format : formats){
//            if(format.format == vk::Format::eR8G8B8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
//                info.surfaceFormat = format;
//                break;
//            }
//        }
//        std::cout << "surface format" << std::endl;
//
//        auto capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
//        info.imageCount = std::clamp<uint32_t>(2, capabilities.minImageCount, capabilities.maxImageCount);
//        info.imageExtent.width = std::clamp<uint32_t>(w, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
//        info.imageExtent.height = std::clamp<uint32_t>(h, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
//        info.transform = capabilities.currentTransform;
//
//        auto presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
//        info.presentMode = vk::PresentModeKHR::eFifo;
//        for(const auto& presentMode : presentModes){
//            if(presentMode == vk::PresentModeKHR::eMailbox){
//                info.presentMode = presentMode;
//                break;
//            }
//        }
//
//        std::cout << "present model" << std::endl;
//    }


    void SwapChain::getImages() {
        images =  device_.getDevice().getSwapchainImagesKHR(swapChain_);
    }

    void SwapChain::createImageViews() {
        imageViews.resize(images.size());

        for(int i = 0; i < images.size(); i++){
            vk::ImageViewCreateInfo createInfo;
            vk::ComponentMapping mapping;       //mapping.setA(vk::ComponentSwizzle::eIdentity);
            vk::ImageSubresourceRange range;
            range.setBaseMipLevel(0)
                .setLevelCount(1)
                .setBaseArrayLayer(0)
                .setLayerCount(1)
                .setAspectMask(vk::ImageAspectFlagBits::eColor);
            createInfo.setImage(images[i])
                    .setViewType(vk::ImageViewType::e2D)
                    .setComponents(mapping)
                    .setFormat(info.surfaceFormat.format)
                    .setSubresourceRange(range);
            imageViews[i] = device_.getDevice().createImageView(createInfo);
        }
    }


} // yic