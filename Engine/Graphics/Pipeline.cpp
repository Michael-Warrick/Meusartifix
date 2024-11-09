#include "Pipeline.hpp"

Graphics::Pipeline::Pipeline(const vk::PhysicalDevice &physicalDevice, const vk::Device &logicalDevice,
                             const vk::Format &format,
                             const vk::SampleCountFlagBits &msaaSamples) : m_PhysicalDevice(physicalDevice),
                                                                           m_LogicalDevice(logicalDevice),
                                                                           m_SwapChainImageFormat(format),
                                                                           m_MSAASamples(msaaSamples) {
    createRenderPass();
}

Graphics::Pipeline::~Pipeline() {
    m_LogicalDevice.destroyRenderPass(m_RenderPass);
}

vk::RenderPass Graphics::Pipeline::GetRenderPass() const {
    return m_RenderPass;
}

void Graphics::Pipeline::createRenderPass() {
    vk::AttachmentDescription colorAttachment = vk::AttachmentDescription()
            .setFormat(m_SwapChainImageFormat)
            .setSamples(m_MSAASamples)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::eColorAttachmentOptimal);

    vk::AttachmentDescription depthAttachment = vk::AttachmentDescription()
            .setFormat(findDepthFormat())
            .setSamples(m_MSAASamples)
            .setLoadOp(vk::AttachmentLoadOp::eClear)
            .setStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    vk::AttachmentDescription colorAttachmentResolve = vk::AttachmentDescription()
            .setFormat(m_SwapChainImageFormat)
            .setSamples(vk::SampleCountFlagBits::e1)
            .setLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStoreOp(vk::AttachmentStoreOp::eStore)
            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
            .setInitialLayout(vk::ImageLayout::eUndefined)
            .setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

    vk::AttachmentReference colorAttachmentReference = vk::AttachmentReference()
            .setAttachment(0)
            .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    vk::AttachmentReference depthAttachmentReference = vk::AttachmentReference()
            .setAttachment(1)
            .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

    vk::AttachmentReference colorAttachmentResolveReference = vk::AttachmentReference()
            .setAttachment(2)
            .setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription subpass = vk::SubpassDescription()
            .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
            .setColorAttachmentCount(1)
            .setPColorAttachments(&colorAttachmentReference)
            .setPDepthStencilAttachment(&depthAttachmentReference)
            .setPResolveAttachments(&colorAttachmentResolveReference);

    vk::SubpassDependency dependency = vk::SubpassDependency()
            .setSrcSubpass(vk::SubpassExternal)
            .setDstSubpass(0)
            .setSrcStageMask(
                    vk::PipelineStageFlagBits::eColorAttachmentOutput |
                    vk::PipelineStageFlagBits::eEarlyFragmentTests)
            .setSrcAccessMask(vk::AccessFlagBits::eNone)
            .setDstStageMask(
                    vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests)
            .setDstAccessMask(
                    vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite);

    std::array<vk::AttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};
    vk::RenderPassCreateInfo renderPassCreateInfo = vk::RenderPassCreateInfo()
            .setAttachmentCount(static_cast<uint32_t>(attachments.size()))
            .setPAttachments(attachments.data())
            .setSubpassCount(1)
            .setPSubpasses(&subpass)
            .setDependencyCount(1)
            .setPDependencies(&dependency);

    vk::Result result = m_LogicalDevice.createRenderPass(&renderPassCreateInfo, nullptr, &m_RenderPass);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to create render pass! Error: " + vk::to_string(result));
    }
}

vk::Format Graphics::Pipeline::findDepthFormat() const {
    return findSupportedFormat(
            {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
            vk::ImageTiling::eOptimal,
            vk::FormatFeatureFlagBits::eDepthStencilAttachment);
}

vk::Format Graphics::Pipeline::findSupportedFormat(const std::vector<vk::Format> &candidates,
                                                   const vk::ImageTiling tiling,
                                                   const vk::FormatFeatureFlags features) const {
    for (const vk::Format format: candidates) {
        vk::FormatProperties properties;

        m_PhysicalDevice.getFormatProperties(format, &properties);
        if (tiling == vk::ImageTiling::eLinear && (properties.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == vk::ImageTiling::eOptimal && (properties.optimalTilingFeatures & features) ==
                                                          features) {
            return format;
        }
    }

    throw std::runtime_error("Failed to find supported format!");
}

