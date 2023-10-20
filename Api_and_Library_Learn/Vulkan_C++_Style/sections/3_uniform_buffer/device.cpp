//
// Created by lenovo on 10/19/2023.
//

#include "device.h"

#ifdef  NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

PFN_vkCreateDebugUtilsMessengerEXT pfnVkCreateDebugUtilsMessengerExt;
PFN_vkDestroyDebugUtilsMessengerEXT pfnVkDestroyDebugUtilsMessengerExt;

VKAPI_ATTR VkResult VKAPI_CALL vkCreateDebugUtilsMessengerEXT(VkInstance instance,
                                                              const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                              const VkAllocationCallbacks* pAllocator,
                                                              VkDebugUtilsMessengerEXT* pMessenger){
    return pfnVkCreateDebugUtilsMessengerExt(instance, pCreateInfo, pAllocator, pMessenger);
}

VKAPI_ATTR void VKAPI_CALL vkDestroyDebugUtilsMessengerEXT(VkInstance instance,
                                                           VkDebugUtilsMessengerEXT messengerExt,
                                                           VkAllocationCallbacks const* pAllocator){
    return pfnVkDestroyDebugUtilsMessengerExt(instance, messengerExt, pAllocator);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserdata){
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;
}

namespace yic{

    Device::Device(Window& window) : window_{window} {
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicDevice();
    }

    Device::~Device() {
        device.destroy();
        instance.destroy(surface);
        if (enableValidationLayers){ instance.destroyDebugUtilsMessengerEXT(debugMessenger); }
        instance.destroy();
    }

    void Device::createInstance() {
        if(enableValidationLayers && !checkValidationLayerSupport()){
            throw std::runtime_error("validation layers required, but not available");
        }

        auto extensions = getRequiredExtensions();
        vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

        vk::ApplicationInfo appInfo{};
        appInfo.setPApplicationName("miku")
                .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
                .setPEngineName("yicvot")
                .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                .setApiVersion(VK_VERSION_1_3);


        vk::InstanceCreateInfo createInfo{};
        createInfo.setPApplicationInfo(&appInfo)
                .setEnabledExtensionCount(extensions.size())
                .setPpEnabledExtensionNames(extensions.data());

        if (enableValidationLayers){
            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.setEnabledLayerCount(validationLayers.size())
                    .setPpEnabledLayerNames(validationLayers.data())
                    .setPNext(&debugCreateInfo);
        } else{
            createInfo.setEnabledLayerCount(0);
        }

        instance = vk::createInstance(createInfo);
    }

    void Device::createSurface() {
        window_.createSurface(instance, &surface);
    }

    void Device::pickPhysicalDevice() {
        std::vector<vk::PhysicalDevice> devices = instance.enumeratePhysicalDevices();
        physicalDevice = devices[0];
        for(const auto& device_ : devices){
            if (device_.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu){
                physicalDevice = device_;
                std::cout << physicalDevice.getProperties().deviceName << std::endl;
                break;
            }
        }
    }

    void Device::createLogicDevice() {
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{};
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority =1.f;
        for(uint32_t queueFamily : uniqueQueueFamilies){
            vk::DeviceQueueCreateInfo createInfo{};
            createInfo.setQueueFamilyIndex(queueFamily)
                    .setQueueCount(1)
                    .setPQueuePriorities(&queuePriority);

            queueCreateInfos.push_back(createInfo);
        }

        vk::PhysicalDeviceFeatures deviceFeatures{};
        vk::DeviceCreateInfo createInfo{};
        createInfo.setQueueCreateInfoCount(queueCreateInfos.size())
                .setPQueueCreateInfos(queueCreateInfos.data())
                .setPEnabledFeatures(&deviceFeatures)
                .setEnabledExtensionCount(deviceExtensions.size())
                .setPpEnabledExtensionNames(deviceExtensions.data());

        if (enableValidationLayers){
            createInfo.setEnabledLayerCount(validationLayers.size())
                    .setPpEnabledLayerNames(validationLayers.data());
        }

        device = physicalDevice.createDevice(createInfo);
        graphicsQueue = device.getQueue(indices.graphicsFamily.value(), 0);
        presentQueue = device.getQueue(indices.presentFamily.value(), 0);
    }


    Device::QueueFamilyIndices Device::findQueueFamilies(const vk::PhysicalDevice &device) {
        QueueFamilyIndices indices;

        std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();

        int i = 0;
        for(const auto& queueFamily : queueFamilies){
            if(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics){
                indices.graphicsFamily = i;
            }

            vk::Bool32 presentSupport = device.getSurfaceSupportKHR(i, surface);
            if(presentSupport){
                indices.presentFamily = i;
            }

            if(indices.isComplete()){
                break;
            }

            i++;
        }

        return indices;
    }


    void Device::setupDebugMessenger() {
        if (!enableValidationLayers) return;

        pfnVkCreateDebugUtilsMessengerExt = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(instance.getProcAddr("vkCreateDebugUtilsMessengerEXT"));
        if(!pfnVkCreateDebugUtilsMessengerExt){
            std::cout << "GetInstanceProcAddr: unable to find pfnVkCreateDebugUtilsMessengerEXT function" << std::endl;
            exit(1);
        }

        pfnVkDestroyDebugUtilsMessengerExt = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(instance.getProcAddr("vkDestroyDebugUtilsMessengerEXT"));
        if(!pfnVkDestroyDebugUtilsMessengerExt){
            std::cout << "GetInstanceProcAddr: Unable to find pfnVkDestroy" << std::endl;
            exit(1);
        }

        vk::DebugUtilsMessengerCreateInfoEXT createInfo{};
        populateDebugMessengerCreateInfo(createInfo);

        debugMessenger = instance.createDebugUtilsMessengerEXT(createInfo);

    }


    void Device::populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT &createInfo) {
        createInfo.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                                      vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                                      vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
                .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                                vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
                                vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation)
                .setPfnUserCallback(debugCallback)
                .setPUserData(nullptr);

    }


    bool Device::checkValidationLayerSupport() {
        std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

        for(const char* layerName : validationLayers){
            bool layerFound = false;

            for(const auto& layerProperties : availableLayers){
                if(strcmp(layerName, layerProperties.layerName) == 0){
                    layerFound = true;
                    break;
                }
            }
            if(!layerFound){
                return false;
            }
        }
        return true;
    }

    std::vector<const char *> Device::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if(enableValidationLayers){
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }


}