//
// Created by lenovo on 10/19/2023.
//

#include "renderer.h"



const int MAX_FRAMES_IN_FLIGHT = 2;

namespace yic{

    Renderer::Renderer(Device& device, SwapChain& swapChain, Pipeline& pipeline) :device_{device}, swapChain_{swapChain}, pipeline_{pipeline}{
        init();
    }

    Renderer::~Renderer() {
        clean();
    }

    void Renderer::init() {

        createFrameBuffers();
        createCommandPool();
        createCommandBuffers();
        createSyncObjects();
    }

    void Renderer::clean() {
        for(auto& swapChainFrameBuffer : swapChainFrameBuffers) { device_.device_().destroy(swapChainFrameBuffer); }


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

        vk::CommandBufferAllocateInfo allocateInfo{};
        allocateInfo.setCommandPool(commandPool)
                .setLevel(vk::CommandBufferLevel::ePrimary)
                .setCommandBufferCount(commandBuffers.size());

        commandBuffers = device_.device_().allocateCommandBuffers(allocateInfo);
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

    ////----------------------------------------------------------------------------
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

//        if (frameBufferResized){
//
//        }

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }
    ////----------------------------------------------------------------------------

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

                commandBuffer.draw(3, 1, 0, 0);
            }
            commandBuffer.endRenderPass();
        }
        commandBuffer.end();

    }






}