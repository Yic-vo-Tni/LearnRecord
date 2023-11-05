//
// Created by lenovo on 10/19/2023.
//

#include "pipeline.h"

namespace yic{

    Pipeline::Pipeline(yic::Device &device) : device_{device} {
        createRenderPass();
        createGraphicsPipeline();
    }

    Pipeline::~Pipeline(){
        device_.device_().destroy(desSetLayout);
        device_.device_().destroy(graphicsPipeline);
        device_.device_().destroy(pipelineLayout);
        device_.device_().destroy(renderPass);
    }


    void Pipeline::createRenderPass() {
        vk::AttachmentDescription colorAttachment{};
        colorAttachment.setFormat(swapChain_->swapChainImageFormat_())
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

    void Pipeline::createGraphicsPipeline() {
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

        auto attribute = LoadVertex::Vertex::vertexAttribute();
        auto vertexBinding = LoadVertex::Vertex::vertexBinding();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.setVertexBindingDescriptionCount(1)
                .setVertexBindingDescriptions(vertexBinding)
                .setVertexAttributeDescriptionCount(1)
                .setVertexAttributeDescriptions(attribute);

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
        desSetLayout = createSetLayout();
        pipelineLayoutInfo.setSetLayoutCount(1)
                .setPSetLayouts(&desSetLayout)
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

    std::vector<char> Pipeline::readFile(const std::string &fileName) {
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

    vk::ShaderModule Pipeline::createShaderModule(const std::vector<char> &code) {
        vk::ShaderModuleCreateInfo createInfo{};
        createInfo.setCodeSize(code.size())
                .setPCode(reinterpret_cast<const uint32_t*>(code.data()));

        vk::ShaderModule shaderModule = device_.device_().createShaderModule(createInfo);
        return shaderModule;
    }

    vk::DescriptorSetLayout Pipeline::createSetLayout() {
        vk::DescriptorSetLayoutCreateInfo createInfo{};
        auto binding = LoadVertex::uniformColor::layoutBinding();
        createInfo.setBindingCount(1)
                .setBindings(binding);

        return device_.device_().createDescriptorSetLayout(createInfo);
    }
}