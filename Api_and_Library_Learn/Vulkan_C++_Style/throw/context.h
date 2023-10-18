//
// Created by lenovo on 10/13/2023.
//

#ifndef LEARN_RECORD_CONTEXT_H
#define LEARN_RECORD_CONTEXT_H

#include "memory"
#include "../src/window.h"
#include "swap_chain.h"

#include "iostream"
#include "optional"

using createSurfaceFunc = std::function<vk::SurfaceKHR(vk::Instance)>;

namespace yic{
    class Context final {
    public:
        struct QueueFamilyIndices final {
            std::optional<uint32_t> graphicsQueue;
            std::optional<uint32_t> presentQueue;

            operator bool() const{
                return graphicsQueue.has_value() && presentQueue.has_value();
            }
        };

        static void init(const createSurfaceFunc& func);
        static void quit();
        static Context& getInstance();

        ~Context();

        vk::Instance instance;
        vk::PhysicalDevice physicalDevice;
        vk::Device device;
        vk::Queue graphicsQueue;
        vk::Queue presentQueue;
        vk::SurfaceKHR surface;
        QueueFamilyIndices queueFamilyIndices;
        std::unique_ptr<SwapChain> swapChain;

        void initSwapChain(int w, int h){
            swapChain = std::make_unique<SwapChain>(w, h);
        }
        void destroySwapChain(){
            swapChain.reset();
        }

        //VkInstance instance;
    private:
        Context(const createSurfaceFunc& func);
        void createInstance();
        void pickPhysicalDevice();
        void createLogicDevice();
        void queryQueueFamilyIndices();
        void getQueue();

        static std::vector<const char*> getRequireExtensions();

        static std::unique_ptr<Context> instance_;

    };
}

#endif //LEARN_RECORD_CONTEXT_H
