//
// Created by lenovo on 10/19/2023.
//

#include "loadVertex.h"

namespace yic{

    LoadVertex::LoadVertex(size_t size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryProperties, yic::Device &device) : device_{device} {
        createBuffer(size, usage);
        MemoryInfo info = queryMemoryInfo(memoryProperties);
        allocateMemory(info);
        bindMemoryToBuffer();
    }

    LoadVertex::~LoadVertex() {
        device_.device_().freeMemory(memory);
        device_.device_().destroyBuffer(buffer);
    }

    void LoadVertex::createBuffer(size_t size, vk::BufferUsageFlags usage) {
        vk::BufferCreateInfo bufferCreateInfo{};
        bufferCreateInfo.setSize(size)
                        .setUsage(usage)
                        .setSharingMode(vk::SharingMode::eExclusive);

        bufferSize = size;
        buffer = device_.device_().createBuffer(bufferCreateInfo);

        if(!buffer){
            throw std::runtime_error("failed to create buffer");
        }
    }

    void LoadVertex::allocateMemory(MemoryInfo info) {
        vk::MemoryAllocateInfo allocateInfo{};
        allocateInfo.setMemoryTypeIndex(info.index)
                    .setAllocationSize(info.size);
        memory = device_.device_().allocateMemory(allocateInfo);
    }

    void LoadVertex::bindMemoryToBuffer() {
        device_.device_().bindBufferMemory(buffer, memory, 0);
    }

    LoadVertex::MemoryInfo LoadVertex::queryMemoryInfo(vk::MemoryPropertyFlags memoryProperties) {
        MemoryInfo info{};
        auto requirements = device_.device_().getBufferMemoryRequirements(buffer);
        info.size = requirements.size;

        auto properties = device_.physicalDevice_().getMemoryProperties();
        for(int i = 0; i < properties.memoryTypeCount; i++){
            if((1 << i) & requirements.memoryTypeBits && properties.memoryTypes[i].propertyFlags & memoryProperties){
                info.index = i;
                break;
            }
        }
        return info;
    }


}