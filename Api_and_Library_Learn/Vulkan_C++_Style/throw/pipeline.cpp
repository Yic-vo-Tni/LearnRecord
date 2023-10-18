//
// Created by lenovo on 10/16/2023.
//

#include "pipeline.h"

namespace yic {

    Pipeline::Pipeline(int w, int h, yic::Shader &shader, yic::Device &device, yic::SwapChain &swapChain) : shader{shader}, device_(device), swapChain_{swapChain} {
//        if (swapChain == nullptr){
//            std::cout << "swapChain is null" << std::endl;
//            swapChain = std::make_unique<SwapChain>(w, h, device);
//        } else{
//            std::cout << "swapChain exist" << std::endl;
//        }
        createRenderPass();
        createLayout();
        createPipeline(w, h);
        std::cout << "Pipeline" << std::endl;
    }

    Pipeline::~Pipeline() {
        device_.getDevice().destroyRenderPass(renderPass);
        device_.getDevice().destroyPipelineLayout(layout);
        device_.getDevice().destroyPipeline(pipeline);
    }

    void Pipeline::createPipeline(int w, int h) {
        vk::GraphicsPipelineCreateInfo createInfo;

        configInfo.inputAssembly.setPrimitiveRestartEnable(false)
                                .setTopology(vk::PrimitiveTopology::eTriangleList);

        auto stages = shader.shaderStage();

        vk::Viewport viewport(0, 0, static_cast<float>(w), static_cast<float>(h), 0, 1);
        vk::Rect2D rect2D({0, 0}, {static_cast<uint32_t>(w), static_cast<uint32_t>(h)});
        configInfo.viewportState.setViewports(viewport)
                                .setScissors(rect2D);

        configInfo.rasterization.setRasterizerDiscardEnable(false)
                                .setDepthBiasEnable(false)
                                .setCullMode(vk::CullModeFlagBits::eBack)
                                .setFrontFace(vk::FrontFace::eCounterClockwise)
                                .setPolygonMode(vk::PolygonMode::eFill)
                                .setLineWidth(1)
                                .setDepthClampEnable(false);

        configInfo.multisample.setSampleShadingEnable(false)
                            .setRasterizationSamples(vk::SampleCountFlagBits::e1);

        configInfo.colorBlendAttach.setBlendEnable(false)
                                    .setColorWriteMask(vk::ColorComponentFlagBits::eA |
                                                    vk::ColorComponentFlagBits::eB |
                                                    vk::ColorComponentFlagBits::eG |
                                                    vk::ColorComponentFlagBits::eR);

        configInfo.colorBlend.setLogicOpEnable(false)
                            .setAttachments(configInfo.colorBlendAttach);

        createInfo.setPVertexInputState(&configInfo.inputState)
                .setPInputAssemblyState(&configInfo.inputAssembly)
                .setStages(stages)
                .setPRasterizationState(&configInfo.rasterization)
                .setPMultisampleState(&configInfo.multisample)
                .setPColorBlendState(&configInfo.colorBlend)
                .setPViewportState(&configInfo.viewportState)
                .setRenderPass(renderPass)
                .setLayout(layout);

        auto result = device_.getDevice().createGraphicsPipeline(nullptr, createInfo);
        if (result.result != vk::Result::eSuccess){
            throw std::runtime_error("failed to create graphics pipeline");
        }
        pipeline = result.value;
    }

    void Pipeline::createLayout() {
        vk::PipelineLayoutCreateInfo createInfo;
        layout = device_.getDevice().createPipelineLayout(createInfo);
    }

    void Pipeline::createRenderPass() {
        vk::RenderPassCreateInfo createInfo;

        vk::AttachmentDescription attachmentDescription;
        attachmentDescription.setFormat(swapChain_.info.surfaceFormat.format)
                            .setInitialLayout(vk::ImageLayout::eUndefined)
                            .setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal)
                            .setLoadOp(vk::AttachmentLoadOp::eClear)
                            .setStoreOp(vk::AttachmentStoreOp::eStore)
                            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                            .setSamples(vk::SampleCountFlagBits::e1);

        vk::AttachmentReference attachmentReference;
        attachmentReference.setLayout(vk::ImageLayout::eColorAttachmentOptimal)
                         .setAttachment(0);
        vk::SubpassDescription subpassDescription;
        subpassDescription.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
                        .setColorAttachments(attachmentReference);

        vk::SubpassDependency subpassDependency;
        subpassDependency.setSrcSubpass(VK_SUBPASS_EXTERNAL)
                        .setDstSubpass(0)
                        .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
                        .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput)
                        .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);

        createInfo.setSubpasses(subpassDescription)
                .setAttachments(attachmentDescription)
                .setDependencies(subpassDependency);

        renderPass = device_.getDevice().createRenderPass(createInfo);
    }



} // yic