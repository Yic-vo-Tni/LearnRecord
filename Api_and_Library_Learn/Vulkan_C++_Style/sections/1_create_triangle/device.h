//
// Created by lenovo on 10/19/2023.
//

#ifndef LEARN_RECORD_DEVICE_H
#define LEARN_RECORD_DEVICE_H

#include "window.h"
#include "optional"
#include "set"
#include <iostream>

namespace yic{
    class Device{
    public:
        struct QueueFamilyIndices{
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool isComplete(){
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        Device(Window& window);
        ~Device();

        auto& device_() { return device; }
        auto& surface_() { return surface; }
        auto& physicalDevice_() { return physicalDevice; }
        auto& graphicsQueue_() { return graphicsQueue; }
        auto& presentQueue_() { return presentQueue; }

        QueueFamilyIndices findQueueFamilies(const vk::PhysicalDevice& device);
    private:
        Window& window_;

        vk::Instance instance;
        vk::DebugUtilsMessengerEXT debugMessenger;
        vk::SurfaceKHR surface;

        vk::PhysicalDevice physicalDevice;
        vk::Device device;

        vk::Queue graphicsQueue;
        vk::Queue presentQueue;

        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicDevice();

        bool checkValidationLayerSupport();
        void populateDebugMessengerCreateInfo(vk::DebugUtilsMessengerCreateInfoEXT& createInfo);
        std::vector<const char*> getRequiredExtensions();
    };
}

#endif //LEARN_RECORD_DEVICE_H
