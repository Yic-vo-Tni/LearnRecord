//
// Created by lenovo on 10/19/2023.
//

#include "renderer.h"

#include <memory>

const int MAX_FRAMES_IN_FLIGHT = 2;

namespace yic{

    Renderer::Renderer(Device& device, SwapChain& swapChain, Pipeline& pipeline) :device_{device}, swapChain_{swapChain}, pipeline_{pipeline}{
        createFrameBuffers();
        createCommandPool();
        createCommandBuffers();
        createVertexBuffer();
        bufferData();
        createUniformBuffer();
        createUniformBufferData();
        createDescriptorPool();
        allocateSets();
        updateSets();
        createSyncObjects();
    }

    Renderer::~Renderer() {
        device_.device_().destroyDescriptorPool(descriptorPool);

        hostUniformBuffer.clear();
        deviceUniformBuffer.clear();

        hostVertexBuffer.reset();
        deviceVertexBuffer.reset();

        for(auto& swapChainFrameBuffer : swapChainFrameBuffers) { device_.device_().destroy(swapChainFrameBuffer); }
        for (auto& semaphore : imageAvailableSemaphores) {device_.device_().destroySemaphore(semaphore);}
        for (auto& semaphore : renderFinishedSemaphores) {device_.device_().destroySemaphore(semaphore);}
        for (auto& fence : inFlightFences) {device_.device_().destroyFence(fence);}

        device_.device_().destroyCommandPool(commandPool);
    }

    void Renderer::createFrameBuffers() {
        swapChainFrameBuffers.resize(swapChain_.swapChainImageViews_().size());

        for(int i = 0; i < swapChain_.swapChainImageViews_().size(); i++){
            vk::ImageView attachments[] = {swapChain_.swapChainImageViews_()[i]};

            vk::FramebufferCreateInfo framebufferCreateInfo{};
            framebufferCreateInfo.setRenderPass(pipeline_.renderPass_())
                    .setAttachmentCount(1)
                    .setPAttachments(attachments)
                    .setWidth(swapChain_.swapChainExtent_().width)
                    .setHeight(swapChain_.swapChainExtent_().height)
                    .setLayers(1);

            swapChainFrameBuffers[i] = device_.device_().createFramebuffer(framebufferCreateInfo);
        }
    }

    void Renderer::createCommandPool() {
        Device::QueueFamilyIndices queueFamilyIndices = device_.findQueueFamilies(device_.physicalDevice_());

        vk::CommandPoolCreateInfo poolCreateInfo{};
        poolCreateInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                .setQueueFamilyIndex(queueFamilyIndices.graphicsFamily.value());

        commandPool = device_.device_().createCommandPool(poolCreateInfo);
    }

    void Renderer::createCommandBuffers() {
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        commandBuffers = device_.device_().allocateCommandBuffers(commandBufferInfo());
    }

    void Renderer::createSyncObjects() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

        vk::SemaphoreCreateInfo semaphoreCreateInfo{};

        vk::FenceCreateInfo fenceCreateInfo{};
        fenceCreateInfo.setFlags(vk::FenceCreateFlagBits::eSignaled);

        for(size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++){
            imageAvailableSemaphores[i] = device_.device_().createSemaphore(semaphoreCreateInfo);
            renderFinishedSemaphores[i] = device_.device_().createSemaphore(semaphoreCreateInfo);
            inFlightFences[i] = device_.device_().createFence(fenceCreateInfo);
        }
    }


    void Renderer::drawFrame() {
        device_.device_().waitForFences(1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        vk::ResultValue<uint32_t> rvImageIndex = device_.device_().acquireNextImageKHR(swapChain_.swapChain_(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE);
        switch (rvImageIndex.result) {
            case vk::Result::eErrorOutOfDateKHR:

                return;
            case vk::Result::eSuccess:
            case vk::Result::eSuboptimalKHR:
                imageIndex = rvImageIndex.value;
                break;
            default:
                throw std::runtime_error("failed to acquire swapChain image!");
                break;
        }

        device_.device_().resetFences(1, &inFlightFences[currentFrame]);

        commandBuffers[currentFrame].reset();
        recordCommandBuffer(commandBuffers[currentFrame], imageIndex);

        vk::Semaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        vk::Semaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        vk::SwapchainKHR swapChains[] = {swapChain_.swapChain_()};

        vk::SubmitInfo submitInfo{};
        submitInfo.setWaitSemaphoreCount(1)
                .setPWaitSemaphores(waitSemaphores)
                .setPWaitDstStageMask(waitStages)
                .setCommandBufferCount(1)
                .setPCommandBuffers(&commandBuffers[currentFrame])
                .setSignalSemaphoreCount(1)
                .setPSignalSemaphores(signalSemaphores);

        device_.graphicsQueue_().submit(submitInfo, inFlightFences[currentFrame]);

        vk::PresentInfoKHR presentInfo{};
        presentInfo.setWaitSemaphoreCount(1)
                .setPWaitSemaphores(signalSemaphores)
                .setSwapchainCount(1)
                .setPSwapchains(swapChains)
                .setPImageIndices(&imageIndex)
                .setPResults(nullptr);

        vk::Result result = device_.presentQueue_().presentKHR(&presentInfo);

        switch (result) {
            case vk::Result::eErrorOutOfDateKHR:
            case vk::Result::eSuboptimalKHR:
                frameBufferResized = false;
                break;
            case vk::Result::eSuccess:
                break;
            default:
                throw std::runtime_error("failed to present swap chain image!");
                break;
        }



        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    vk::CommandBufferAllocateInfo Renderer::commandBufferInfo() {
        vk::CommandBufferAllocateInfo allocateInfo{};
        allocateInfo.setCommandPool(commandPool)
                .setLevel(vk::CommandBufferLevel::ePrimary)
                .setCommandBufferCount(commandBuffers.size());

        return allocateInfo;
    }


    void Renderer::recordCommandBuffer(vk::CommandBuffer &commandBuffer, uint32_t imageIndex) {
        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit)
                .setPInheritanceInfo(nullptr);

        commandBuffer.begin(beginInfo);
        {
            vk::RenderPassBeginInfo renderPassBeginInfo{};
            renderPassBeginInfo.setRenderPass(pipeline_.renderPass_())
                    .setFramebuffer(swapChainFrameBuffers[imageIndex])
                    .setRenderArea(vk::Rect2D(vk::Offset2D(0, 0), swapChain_.swapChainExtent_()));

            vk::ClearValue clearValue = vk::ClearColorValue(1.f, 0.71f, 0.75f, 1.f);
            renderPassBeginInfo.setClearValueCount(1)
                    .setPClearValues(&clearValue);

            commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
            {
                commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline_.graphicsPipeline_());

                vk::Viewport viewport{};
                viewport.setX(0.f)
                        .setY(0.f)
                        .setWidth(static_cast<float>(swapChain_.swapChainExtent_().width))
                        .setHeight(static_cast<float>(swapChain_.swapChainExtent_().height))
                        .setMinDepth(0.f)
                        .setMaxDepth(1.f);

                commandBuffer.setViewport(0, 1, &viewport);

                vk::Rect2D scissor{};
                scissor.setOffset(vk::Offset2D(0, 0))
                        .setExtent(swapChain_.swapChainExtent_());

                commandBuffer.setScissor(0, 1, &scissor);

                vk::DeviceSize offset = 0;
                commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, pipeline_.pipelineLayout_(), 0, desSet[currentFrame], {});
                commandBuffer.bindVertexBuffers(0, deviceVertexBuffer->buffer_(), offset);

                commandBuffer.draw(3, 1, 0, 0);
            }
            commandBuffer.endRenderPass();
        }
        commandBuffer.end();

    }


    void Renderer::createVertexBuffer() {
        hostVertexBuffer = std::make_unique<LoadVertex>(sizeof(vertices) , vk::BufferUsageFlagBits::eTransferSrc | vk::BufferUsageFlagBits::eVertexBuffer,
                                          vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, device_);
        deviceVertexBuffer = std::make_unique<LoadVertex>(sizeof(vertices), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                                                vk::MemoryPropertyFlagBits::eDeviceLocal, device_);
    }

    void Renderer::bufferData() {
        void *ptr =  device_.device_().mapMemory(hostVertexBuffer->memory_(), 0, hostVertexBuffer->size_());
        memcpy(ptr, vertices.data(), sizeof(vertices));
        device_.device_().unmapMemory(hostVertexBuffer->memory_());

        auto commandBuffers_ = device_.device_().allocateCommandBuffers(commandBufferInfo());

        for(const auto& commandBuffer : commandBuffers_){

            vk::CommandBufferBeginInfo beginInfo{};
            beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

            commandBuffer.begin(beginInfo);{
                vk::BufferCopy bufferCopy{};
                bufferCopy.setSize(deviceVertexBuffer->size_())
                        .setSrcOffset(0)
                        .setDstOffset(0);
                commandBuffer.copyBuffer(hostVertexBuffer->buffer_(), deviceVertexBuffer->buffer_(), bufferCopy);
            } commandBuffer.end();

            vk::SubmitInfo submitInfo{};
            submitInfo.setCommandBuffers(commandBuffer);
            device_.graphicsQueue_().submit(submitInfo);

            device_.device_().waitIdle();


            freeCommandBuffer(commandBuffer);
        }
    }

    inline void Renderer::freeCommandBuffer(vk::CommandBuffer buffer) {
        device_.device_().freeCommandBuffers(commandPool, buffer);
        commandBuffers.clear();
    }

    void Renderer::createUniformBuffer() {
        hostUniformBuffer.resize(MAX_FRAMES_IN_FLIGHT);
        deviceUniformBuffer.resize(MAX_FRAMES_IN_FLIGHT);

        for(auto& buffer : hostUniformBuffer){
            buffer = std::make_unique<LoadVertex>(sizeof(uniformColor), vk::BufferUsageFlagBits::eTransferSrc,
                                         vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent, device_);
        }

        for(auto& buffer : deviceUniformBuffer){
            buffer = std::make_unique<LoadVertex>(sizeof(uniformColor), vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eUniformBuffer,
                                                  vk::MemoryPropertyFlagBits::eDeviceLocal, device_);
        }
    }


    void Renderer::createUniformBufferData() {
        for(int i = 0; i < hostUniformBuffer.size(); i++){
            auto& buffer = hostUniformBuffer[i];
            void* ptr = device_.device_().mapMemory(buffer->memory_(), 0, buffer->size_());
            memcpy(ptr, &uniformColor, sizeof(uniformColor));
            device_.device_().unmapMemory(buffer->memory_());

            copyBuffer(buffer->buffer_(), deviceUniformBuffer[i]->buffer_(), buffer->size_(), 0, 0);
        }
    }


    void Renderer::copyBuffer(vk::Buffer &src, vk::Buffer &dst, size_t size, size_t srcOffset, size_t dstOffset) {
        auto cmdInfo = commandBufferInfo();
        cmdInfo.setCommandBufferCount(size);
        auto commandBuffers_ = device_.device_().allocateCommandBuffers(cmdInfo);

        vk::CommandBufferBeginInfo beginInfo{};
        beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);

        for(auto& commandBuffer : commandBuffers_){
            commandBuffer.begin(beginInfo);{
                vk::BufferCopy bufferCopy{};
                bufferCopy.setSize(size)
                        .setSrcOffset(srcOffset)
                        .setDstOffset(dstOffset);
                commandBuffer.copyBuffer(src, dst, bufferCopy);
            }commandBuffer.end();

            vk::SubmitInfo submitInfo{};
            submitInfo.setCommandBuffers(commandBuffer);
            device_.graphicsQueue_().submit(submitInfo);

            device_.device_().waitIdle();

            freeCommandBuffer(commandBuffer);
        }
    }

    void Renderer::createDescriptorPool() {
        vk::DescriptorPoolCreateInfo desPoolInfo{};
        vk::DescriptorPoolSize desPoolSize{};
        desPoolSize.setType(vk::DescriptorType::eUniformBuffer)
                   .setDescriptorCount(MAX_FRAMES_IN_FLIGHT) ;
        desPoolInfo.setMaxSets(MAX_FRAMES_IN_FLIGHT)
                   .setPoolSizes(desPoolSize);

        descriptorPool = device_.device_().createDescriptorPool(desPoolInfo);
    }


    void Renderer::allocateSets() {
        std::vector<vk::DescriptorSetLayout> setLayouts{MAX_FRAMES_IN_FLIGHT, pipeline_.desSetLayout_()};
        vk::DescriptorSetAllocateInfo desSetAllocInfo{};
        desSetAllocInfo.setDescriptorPool(descriptorPool)
                       .setDescriptorSetCount(MAX_FRAMES_IN_FLIGHT)
                       .setSetLayouts(setLayouts);

        desSet = device_.device_().allocateDescriptorSets(desSetAllocInfo);
    }

    void Renderer::updateSets() {
        for(int i = 0; i < desSet.size(); i++){
            auto& set = desSet[i];
            vk::DescriptorBufferInfo desBufferInfo{};
            desBufferInfo.setBuffer(deviceUniformBuffer[i]->buffer_())
                        .setOffset(0)
                        .setRange(deviceUniformBuffer[i]->size_());


            vk::WriteDescriptorSet writeSet{};
            writeSet.setDescriptorCount(1)
                    .setDescriptorType(vk::DescriptorType::eUniformBuffer)
                    .setBufferInfo(desBufferInfo)
                    .setDstBinding(0)
                    .setDstSet(set)
                    .setDstArrayElement(0);

            device_.device_().updateDescriptorSets(writeSet, {});
        }
    }

}