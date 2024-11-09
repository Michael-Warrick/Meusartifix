#include "GraphicsPipeline.hpp"

Graphics::GraphicsPipeline::GraphicsPipeline(const vk::Device &logicalDevice,
                                             const vk::SampleCountFlagBits &msaaSamples) : m_LogicalDevice(
        logicalDevice), m_MSAASamples(msaaSamples) {

}

void Graphics::GraphicsPipeline::createGraphicsPipeline(const std::string &vertexShaderFilePath,
                                                        const std::string &fragmentShaderFilePath) {
    auto vertexShaderCode = IO::File::Read(vertexShaderFilePath);
    auto fragmentShaderCode = IO::File::Read(fragmentShaderFilePath);

    vk::ShaderModule vertexShaderModule = createShaderModule(vertexShaderCode);
    vk::ShaderModule fragmentShaderModule = createShaderModule(fragmentShaderCode);

    vk::PipelineShaderStageCreateInfo vertexShaderStageInfo = vk::PipelineShaderStageCreateInfo()
            .setStage(vk::ShaderStageFlagBits::eVertex)
            .setModule(vertexShaderModule)
            .setPName("main");
    vk::PipelineShaderStageCreateInfo fragmentShaderStageInfo = vk::PipelineShaderStageCreateInfo()
            .setStage(vk::ShaderStageFlagBits::eFragment)
            .setModule(fragmentShaderModule)
            .setPName("main");

    vk::PipelineShaderStageCreateInfo shaderStages[] = {vertexShaderStageInfo, fragmentShaderStageInfo};
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo = vk::PipelineVertexInputStateCreateInfo()
            .setVertexBindingDescriptionCount(1)
            .setVertexAttributeDescriptionCount(static_cast<uint32_t>(attributeDescriptions.size()))
            .setPVertexBindingDescriptions(&bindingDescription)
            .setPVertexAttributeDescriptions(attributeDescriptions.data());

    vk::PipelineInputAssemblyStateCreateInfo inputAssembly = vk::PipelineInputAssemblyStateCreateInfo()
            .setTopology(vk::PrimitiveTopology::eTriangleList)
            .setPrimitiveRestartEnable(vk::False);

    vk::PipelineViewportStateCreateInfo viewportState = vk::PipelineViewportStateCreateInfo()
            .setViewportCount(1)
            .setScissorCount(1);

    vk::PipelineRasterizationStateCreateInfo rasterizer = vk::PipelineRasterizationStateCreateInfo()
            .setDepthBiasEnable(vk::False)
            .setRasterizerDiscardEnable(vk::False)
            .setPolygonMode(vk::PolygonMode::eFill)
            .setLineWidth(1.0f)
            .setCullMode(vk::CullModeFlagBits::eBack)
            .setFrontFace(vk::FrontFace::eClockwise)
            .setDepthBiasEnable(vk::False);

    vk::PipelineMultisampleStateCreateInfo multisampling = vk::PipelineMultisampleStateCreateInfo()
            .setSampleShadingEnable(vk::False)
            .setRasterizationSamples(m_MSAASamples);

    vk::PipelineDepthStencilStateCreateInfo depthStencil = vk::PipelineDepthStencilStateCreateInfo()
            .setDepthTestEnable(vk::True)
            .setDepthWriteEnable(vk::True)
            .setDepthCompareOp(vk::CompareOp::eLess)
            .setDepthBoundsTestEnable(vk::False)
            .setStencilTestEnable(vk::False);

    vk::PipelineColorBlendAttachmentState colorBlendAttachment = vk::PipelineColorBlendAttachmentState()
            .setColorWriteMask(
                    vk::ColorComponentFlagBits::eR |
                    vk::ColorComponentFlagBits::eG |
                    vk::ColorComponentFlagBits::eB |
                    vk::ColorComponentFlagBits::eA)
            .setBlendEnable(vk::True);
}

vk::ShaderModule Graphics::GraphicsPipeline::createShaderModule(const std::vector<char> &code) {
    vk::ShaderModuleCreateInfo createInfo = vk::ShaderModuleCreateInfo()
            .setCodeSize(code.size())
            .setPCode(reinterpret_cast<const uint32_t *>(code.data()));

    vk::ShaderModule shaderModule;
    vk::Result result = m_LogicalDevice.createShaderModule(&createInfo, nullptr, &shaderModule);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to create shader module! Error: " + vk::to_string(result));
    }
}
