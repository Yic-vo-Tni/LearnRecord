//
// Created by lenovo on 10/19/2023.
//

#ifndef LEARN_RECORD_LOADVERTEX_H
#define LEARN_RECORD_LOADVERTEX_H

#include "vulkan/vulkan.hpp"
#include "device.h"

namespace yic {

    class LoadVertex {
    public:
        struct Vertex final {
            float x, y;

            static vk::VertexInputAttributeDescription vertexAttribute(){
                vk::VertexInputAttributeDescription vertAttr{};
                vertAttr.setBinding(0)
                        .setFormat(vk::Format::eR32G32Sfloat)
                        .setLocation(0)
                        .setOffset(0);

                return vertAttr;
            };

            static vk::VertexInputBindingDescription vertexBinding(){
                vk::VertexInputBindingDescription vertBind{};
                vertBind.setBinding(0)
                        .setInputRate(vk::VertexInputRate::eVertex)
                        .setStride(sizeof(Vertex));

                return vertBind;
            }
        };

        struct Color final {
            float r, g, b;
        };

        struct uniformColor final {
            Color color{};

            static vk::DescriptorSetLayoutBinding layoutBinding(){
                vk::DescriptorSetLayoutBinding binding{};
                binding.setBinding(0)
                        .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                        .setStageFlags(vk::ShaderStageFlagBits::eFragment)
                        .setDescriptorCount(1);

                return binding;
            }
        };

        LoadVertex(size_t size, vk::BufferUsageFlags usage, vk::MemoryPropertyFlags memoryProperties, Device& device);
        ~LoadVertex();

        auto& memory_() { return memory; }
        auto& size_() { return bufferSize; }
        auto& buffer_() { return buffer; }

    private:
        struct MemoryInfo final {
            size_t size;
            uint32_t index;
        };

        Device& device_;

        vk::Buffer buffer;
        vk::DeviceMemory memory;
        size_t bufferSize;

        void createBuffer(size_t size, vk::BufferUsageFlags usage);
        void allocateMemory(MemoryInfo info);
        void bindMemoryToBuffer();

        MemoryInfo queryMemoryInfo(vk::MemoryPropertyFlags );

    };

}
#endif //LEARN_RECORD_LOADVERTEX_H
