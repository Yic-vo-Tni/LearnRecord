//
// Created by lenovo on 10/13/2023.
//
#include "context.h"

#include <memory>
#include <utility>

namespace yic {
    std::unique_ptr<Context> Context::instance_ = nullptr;

    void Context::init(const createSurfaceFunc& func) {
        instance_.reset(new Context(func));
    }

    void Context::quit() {
        instance_.reset();
    }

    Context &Context::getInstance() {
        return *instance_;
    }

    Context::Context(const createSurfaceFunc& func) {
        createInstance();
        pickPhysicalDevice();
        surface = func(instance);
        queryQueueFamilyIndices();
        createLogicDevice();
        getQueue();
    }

    Context::~Context() {
        device.destroy();
        instance.destroy();
    }

    void Context::createInstance() {
        vk::InstanceCreateInfo createInfo;
        std::vector<const char *> layers = {"VK_LAYER_KHRONOS_validation"};

//        auto layers = vk::enumerateInstanceLayerProperties();
//        for(auto layer : layers){
//            std::cout << layer.layerName << std::endl;
//        }

        vk::ApplicationInfo appInfo;
        auto extensions = getRequireExtensions();
        appInfo.setApiVersion(VK_API_VERSION_1_3);
        createInfo.setPApplicationInfo(&appInfo)
                .setPEnabledLayerNames(layers)
                .setPEnabledExtensionNames(extensions);

        //createInfo{.sType = };
                //.sType = static_cast<vk::StructureType>(VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO);

//        for(auto& extension : extensions){
//            std::cout << extension << std::endl;
//        }

        instance = vk::createInstance(createInfo);
    }

    void Context::pickPhysicalDevice() {
        auto devices = instance.enumeratePhysicalDevices();
        physicalDevice = devices[0];
        for(auto device_ : devices) {
            if (device_.getProperties().deviceType == vk::PhysicalDeviceType::eDiscreteGpu){
                physicalDevice = device_;
                std::cout << physicalDevice.getProperties().deviceName << std::endl;
                return;
            }
        }

        std::cout << physicalDevice.getProperties().deviceName;
    }

    void Context::createLogicDevice() {
        std::array<const char*, 1> extensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        vk::DeviceCreateInfo createInfo;
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        float priorities = 1.f;
        if (queueFamilyIndices.graphicsQueue.value() == queueFamilyIndices.presentQueue.value()){
            vk::DeviceQueueCreateInfo queueCreateInfo;
            queueCreateInfo.setPQueuePriorities(&priorities)
                    .setQueueCount(1)
                    .setQueueFamilyIndex(queueFamilyIndices.graphicsQueue.value());
            queueCreateInfos.push_back(std::move(queueCreateInfo));
        } else {
            vk::DeviceQueueCreateInfo queueCreateInfo;
            queueCreateInfo.setPQueuePriorities(&priorities)
                    .setQueueCount(1)
                    .setQueueFamilyIndex(queueFamilyIndices.graphicsQueue.value());
            queueCreateInfos.push_back(queueCreateInfo);
            queueCreateInfo.setQueuePriorities(priorities)
                    .setQueueCount(1)
                    .setQueueFamilyIndex(queueFamilyIndices.presentQueue.value());
            queueCreateInfos.push_back(queueCreateInfo);
        }
        createInfo.setQueueCreateInfos(queueCreateInfos)
                .setPpEnabledExtensionNames(extensions.data());

        device = physicalDevice.createDevice(createInfo);
    }

    void Context::queryQueueFamilyIndices() {
        auto properties = physicalDevice.getQueueFamilyProperties();
        for(int i = 0; i < properties.size(); i++){
            const auto& property = properties[i];
            if (property.queueFlags | vk::QueueFlagBits::eGraphics){
                queueFamilyIndices.graphicsQueue = i;
                break;
            }
            if (physicalDevice.getSurfaceSupportKHR(i, surface)){
                queueFamilyIndices.presentQueue = i;
            }
            if (queueFamilyIndices){
                break;
            }
        }
    }

    void Context::getQueue() {
        graphicsQueue = device.getQueue(queueFamilyIndices.graphicsQueue.value(), 0);
        presentQueue = device.getQueue(queueFamilyIndices.presentQueue.value(), 0);
    }

    std::vector<const char *> Context::getRequireExtensions() {
        uint32_t count{};
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&count);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);

        return extensions;
    }














}