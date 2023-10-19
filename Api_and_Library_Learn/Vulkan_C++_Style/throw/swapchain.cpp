//
// Created by lenovo on 10/19/2023.
//

#include "renderer.h"



const int MAX_FRAMES_IN_FLIGHT = 2;

namespace yic{

    Renderer::Renderer(Window &window, Device& device) : window_{window}, device_{device}{
        init();
    }

    Renderer::~Renderer() {
        clean();
    }

    void Renderer::init() {
        createSwapChain();
        createImageViews();
        createRenderPass();
        createGraphicsPipeline();
        createFrameBuffers();
        createCommandPool();
        createCommandBuffers();
        createSyncObjects();
    }

    void Renderer::clean() {
        for(auto& swapChainFrameBuffer : swapChainFrameBuffers) { device_.device_().destroy(swapChainFrameBuffer); }
        for(auto& swapChainImageView : swapChainImageViews) { device_.device_().destroy(swapChainImageView); }

        device_.device_().destroyCommandPool(commandPool);
        device_.device_().destroySwapchainKHR(swapChain);
        device_.device_().destroy(graphicsPipeline);
        device_.device_().destroy(pipelineLayout);
        device_.device_().destroy(renderPass);



    }





    void Renderer::createSwapChain() {
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

    void Renderer::createImageViews() {
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

    void Renderer::createRenderPass() {
        vk::AttachmentDescription colorAttachment{};
        colorAttachment.setFormat(swapChainImageFormat)
                .setSamples(vk::SampleCountFlagBits::e1)
                .setLoadOp(vk::AttachmentLoadOp::eClear)
                .setStoreOp(vk::AttachmentStoreOp::eStore)
                .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                .setInitialLayout(vk::ImageLayout::eUndefined)
                .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

        vk::AttachmentReference colorAttachmentRef{};
        colorAttachmentRef.setAttachment(0)
                .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

        vk::SubpassDescription subpass{};
        subpass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                .setColorAttachmentCount(1)
                .setPColorAttachments(&colorAttachmentRef);

        vk::SubpassDependency dependency{};
        dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL)
                .setDstSubpass(0)
                .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                .setSrcAccessMask(vk::AccessFlagBits::eNone)
                .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

        vk::RenderPassCreateInfo renderPassInfo{};
        renderPassInfo.setAttachmentCount(1)
                .setPAttachments(&colorAttachment)
                .setSubpassCount(1)
                .setPSubpasses(&subpass)
                .setDependencyCount(1)
                .setPDependencies(&dependency);

        renderPass = device_.device_().createRenderPass(renderPassInfo);
    }

    void Renderer::createGraphicsPipeline() {
        auto vertShaderCode = readFile("vertexShader.vert.spv");
        auto fragShaderCode = readFile("fragmentShader.frag.spv");

        vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        vk::PipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.setStage(vk::ShaderStageFlagBits::eVertex)
                .setModule(vertShaderModule)
                .setPName("main");
        vk::PipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.setStage(vk::ShaderStageFlagBits::eFragment)
                .setModule(fragShaderModule)
                .setPName("main");

        vk::PipelineShaderStageCreateInfo shaderStage[] = {vertShaderStageInfo, fragShaderStageInfo};

        std::vector<vk::DynamicState> dynamicsStates = {
                vk::DynamicState::eViewport,
                vk::DynamicState::eScissor
        };

        vk::PipelineDynamicStateCreateInfo dynamicInfo{};
        dynamicInfo.setDynamicStateCount(dynamicsStates.size())
                .setPDynamicStates(dynamicsStates.data());

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.setVertexBindingDescriptionCount(0)
                .setPVertexBindingDescriptions(nullptr)
                .setVertexAttributeDescriptionCount(0)
                .setVertexAttributeDescriptions(nullptr);

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.setTopology(vk::PrimitiveTopology::eTriangleList)
                .setPrimitiveRestartEnable(VK_FALSE);

        vk::PipelineViewportStateCreateInfo viewportState{};
        viewportState.setViewportCount(1)
                .setScissorCount(1);

        vk::PipelineRasterizationStateCreateInfo rasterization{};
        rasterization.setDepthClampEnable(VK_FALSE)
                .setRasterizerDiscardEnable(VK_FALSE)
                .setPolygonMode(vk::PolygonMode::eFill)
                .setLineWidth(1.f)
                .setCullMode(vk::CullModeFlagBits::eBack)
                .setFrontFace(vk::FrontFace::eClockwise)
                .setDepthBiasEnable(VK_FALSE)
                .setDepthBiasConstantFactor(0.f)
                .setDepthBiasClamp(0.f)
                .setDepthBiasSlopeFactor(0.f);

        vk::PipelineMultisampleStateCreateInfo multisample{};
        multisample.setSampleShadingEnable(VK_FALSE)
                .setRasterizationSamples(vk::SampleCountFlagBits::e1)
                .setMinSampleShading(1.f)
                .setPSampleMask(nullptr)
                .setAlphaToCoverageEnable(VK_FALSE)
                .setAlphaToOneEnable(VK_FALSE);

        vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                               vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
                .setBlendEnable(VK_FALSE)
                .setSrcColorBlendFactor(vk::BlendFactor::eOne)
                .setDstColorBlendFactor(vk::BlendFactor::eZero)
                .setColorBlendOp(vk::BlendOp::eAdd)
                .setSrcAlphaBlendFactor(vk::BlendFactor::eOne)
                .setDstAlphaBlendFactor(vk::BlendFactor::eZero)
                .setAlphaBlendOp(vk::BlendOp::eAdd);

        std::array<float, 4> blendConstants = {0.f, 0.f, 0.f, 0.f};

        vk::PipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.setLogicOpEnable(VK_FALSE)
                .setLogicOp(vk::LogicOp::eCopy)
                .setAttachmentCount(1)
                .setPAttachments(&colorBlendAttachment)
                .setBlendConstants(blendConstants);

        vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.setSetLayoutCount(0)
                .setPSetLayouts(nullptr)
                .setPushConstantRangeCount(0)
                .setPPushConstantRanges(nullptr);

        pipelineLayout = device_.device_().createPipelineLayout(pipelineLayoutInfo);

        vk::GraphicsPipelineCreateInfo pipelineCreateInfo{};
        pipelineCreateInfo.setStageCount(2)
                .setPStages(shaderStage)
                .setPVertexInputState(&vertexInputInfo)
                .setPInputAssemblyState(&inputAssembly)
                .setPViewportState(&viewportState)
                .setPRasterizationState(&rasterization)
                .setPMultisampleState(&multisample)
                .setPDepthStencilState(nullptr)
                .setPColorBlendState(&colorBlending)
                .setPDynamicState(&dynamicInfo)
                .setLayout(pipelineLayout)
                .setRenderPass(renderPass)
                .setSubpass(0)
                .setBasePipelineHandle(VK_NULL_HANDLE)
                .setBasePipelineIndex(-1);

        vk::ResultValue<vk::Pipeline> defaultPipeline = device_.device_().createGraphicsPipeline(VK_NULL_HANDLE, pipelineCreateInfo);
        switch (defaultPipeline.result) {
            case vk::Result::eSuccess: graphicsPipeline = std::move(defaultPipeline.value);
                break;
            default:
                assert(false);
        }

        device_.device_().destroy(vertShaderModule);
        device_.device_().destroy(fragShaderModule);
    }

    void Renderer::createFrameBuffers() {
        swapChainFrameBuffers.resize(swapChainImageViews.size());

        for(int i = 0; i < swapChainImageViews.size(); i++){
            vk::ImageView attachments[] = {swapChainImageViews[i]};

            vk::FramebufferCreateInfo framebufferCreateInfo{};
            framebufferCreateInfo.setRenderPass(renderPass)
                    .setAttachmentCount(1)
                    .setPAttachments(attachments)
                    .setWidth(swapChainExtent.width)
                    .setHeight(swapChainExtent.height)
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
        vk::ResultValue<uint32_t> rvImageIndex = device_.device_().acquireNextImageKHR(swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE);
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
        vk::SwapchainKHR swapChains[] = {swapChain};

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
            renderPassBeginInfo.setRenderPass(renderPass)
                    .setFramebuffer(swapChainFrameBuffers[imageIndex])
                    .setRenderArea(vk::Rect2D(vk::Offset2D(0, 0), swapChainExtent));

            vk::ClearValue clearValue = vk::ClearColorValue(1.f, 0.71f, 0.75f, 1.f);
            renderPassBeginInfo.setClearValueCount(1)
                    .setPClearValues(&clearValue);

            commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
            {
                commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, graphicsPipeline);

                vk::Viewport viewport{};
                viewport.setX(0.f)
                        .setY(0.f)
                        .setWidth(static_cast<float>(swapChainExtent.width))
                        .setHeight(static_cast<float>(swapChainExtent.height))
                        .setMinDepth(0.f)
                        .setMaxDepth(1.f);

                commandBuffer.setViewport(0, 1, &viewport);

                vk::Rect2D scissor{};
                scissor.setOffset(vk::Offset2D(0, 0))
                        .setExtent(swapChainExtent);

                commandBuffer.setScissor(0, 1, &scissor);

                commandBuffer.draw(3, 1, 0, 0);
            }
            commandBuffer.endRenderPass();
        }
        commandBuffer.end();

    }

    vk::ShaderModule Renderer::createShaderModule(const std::vector<char> &code) {
        vk::ShaderModuleCreateInfo createInfo{};
        createInfo.setCodeSize(code.size())
                .setPCode(reinterpret_cast<const uint32_t*>(code.data()));

        vk::ShaderModule shaderModule = device_.device_().createShaderModule(createInfo);
        return shaderModule;
    }

    std::vector<char> Renderer::readFile(const std::string &fileName) {
        std::ifstream file(fileName, std::ios::ate | std::ios::binary);

        if(!file.is_open()){
            throw std::runtime_error("failed to open file");
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    Renderer::SwapChainSupportDetails Renderer::querySwapChainSupport(const vk::PhysicalDevice &device) {
        SwapChainSupportDetails details{};

        details.capabilities = device.getSurfaceCapabilitiesKHR(device_.surface_());
        details.formats = device.getSurfaceFormatsKHR(device_.surface_());
        details.presentModes = device.getSurfacePresentModesKHR(device_.surface_());

        return details;
    }


    vk::SurfaceFormatKHR Renderer::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
        for(const auto& available : availableFormats){
            if(available.format == vk::Format::eR8G8B8A8Srgb && available.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
                return available;
            }
        }

        return availableFormats[0];
    }

    vk::PresentModeKHR Renderer::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
        for(const auto& available : availablePresentModes){
            if(available == vk::PresentModeKHR::eMailbox){
                return available;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D Renderer::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) {
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