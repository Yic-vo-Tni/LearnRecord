//
// Created by lenovo on 10/15/2023.
//


#include "device.h"

#ifdef  NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    const int MAX_FRAMES_IN_FLIGHT = 2;

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
        //std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

        return VK_FALSE;
    }


namespace yic{

    Device::Device(Window &window) : window_{window}{
        init();
    }

    Device::~Device() {
        clean();
    }

    void Device::init() {
        createInstance();
        setupDebugMessenger();
        createSurface();
        pickPhysicalDevice();
        createLogicDevice();
        createSwapChain();
        createImageViews();
        createRenderPass();
        createGraphicsPipeline();
    }

    void Device::clean() {
        for(auto& swapChainImageView : swapChainImageViews){ device.destroy(swapChainImageView); }

        device.destroySwapchainKHR(swapChain);

        device.destroy(graphicsPipeline);
        device.destroy(pipelineLayout);
        device.destroy(renderPass);

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

    void Device::createSwapChain() {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);

        vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        vk::Extent2D extent2D = chooseSwapExtent(swapChainSupport.capabilities);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if(swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount){
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        vk::SwapchainCreateInfoKHR createInfo{};
        createInfo.setSurface(surface)
                .setMinImageCount(imageCount)
                .setImageFormat(surfaceFormat.format)
                .setImageColorSpace(surfaceFormat.colorSpace)
                .setImageExtent(extent2D)
                .setImageArrayLayers(1)
                .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
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

        swapChain = device.createSwapchainKHR(createInfo);
        swapChainImages = device.getSwapchainImagesKHR(swapChain);
        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent2D;
    }

    void Device::createImageViews() {
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

            swapChainImageViews[i] = device.createImageView(createInfo);
        }
    }

    void Device::createRenderPass() {
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

        renderPass = device.createRenderPass(renderPassInfo);
    }

    void Device::createGraphicsPipeline() {
        auto vertShaderCode = readFile(R"(F:\Program\LearningRecord\Api_and_Library_Learn\Vulkan_C++_Style\shader\vertexShader.vert.spv)");
        auto fragShaderCode = readFile(R"(F:\Program\LearningRecord\Api_and_Library_Learn\Vulkan_C++_Style\shader\fragmentShader.frag.spv)");

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
        colorBlendAttachment.setColorWriteMask(vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eB |
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

        pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo);

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

        vk::ResultValue<vk::Pipeline> defaultPipeline = device.createGraphicsPipeline(VK_NULL_HANDLE, pipelineCreateInfo);
        switch (defaultPipeline.result) {
            case vk::Result::eSuccess: graphicsPipeline = std::move(defaultPipeline.value);
                break;
            default:
                assert(false);
        }

        device.destroy(vertShaderModule);
        device.destroy(fragShaderModule);
    }

    void Device::createFrameBuffers() {
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

            swapChainFrameBuffers[i] = device.createFramebuffer(framebufferCreateInfo);
        }
    }

    void Device::createCommandPool() {
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

        vk::CommandPoolCreateInfo poolCreateInfo{};
        poolCreateInfo.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                    .setQueueFamilyIndex(queueFamilyIndices.graphicsFamily.value());

        commandPool = device.createCommandPool(poolCreateInfo);
    }

    void Device::createCommandBuffers() {
        commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

        vk::CommandBufferAllocateInfo allocateInfo{};
        allocateInfo.setCommandPool(commandPool)
                    .setLevel(vk::CommandBufferLevel::ePrimary)
                    .setCommandBufferCount(commandBuffers.size());

        commandBuffers = device.allocateCommandBuffers(allocateInfo);
    }



    ////----------------------------------------------------------------------------
    void Device::x() {}
    ////----------------------------------------------------------------------------



    vk::ShaderModule Device::createShaderModule(const std::vector<char> &code) {
        vk::ShaderModuleCreateInfo createInfo{};
        createInfo.setCodeSize(code.size())
                .setPCode(reinterpret_cast<const uint32_t*>(code.data()));

        vk::ShaderModule shaderModule = device.createShaderModule(createInfo);
        return shaderModule;
    }

    std::vector<char> Device::readFile(const std::string &fileName) {
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

    Device::SwapChainSupportDetails Device::querySwapChainSupport(const vk::PhysicalDevice &device) {
        SwapChainSupportDetails details{};

        details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
        details.formats = device.getSurfaceFormatsKHR(surface);
        details.presentModes = device.getSurfacePresentModesKHR(surface);

        return details;
    }


    vk::SurfaceFormatKHR Device::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
        for(const auto& available : availableFormats){
            if(available.format == vk::Format::eR8G8B8A8Srgb && available.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
                return available;
            }
        }

        return availableFormats[0];
    }

    vk::PresentModeKHR Device::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
        for(const auto& available : availablePresentModes){
            if(available == vk::PresentModeKHR::eMailbox){
                return available;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D Device::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &capabilities) {
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