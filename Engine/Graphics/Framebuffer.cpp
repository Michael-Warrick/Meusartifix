#include "Framebuffer.hpp"

Graphics::Framebuffer::Framebuffer(const vk::Device &logicalDevice, const std::vector<vk::ImageView> &imageViews,
                                   const vk::RenderPass &renderPass, const uint32_t &width, const uint32_t &height)
        : m_ImageViews(imageViews), m_RenderPass(renderPass), m_Width(width), m_Height(height) {

}

Graphics::Framebuffer::~Framebuffer() {
    for (auto framebuffer : m_Framebuffers) {
        m_LogicalDevice.destroyFramebuffer();
    }
}

void Graphics::Framebuffer::createFramebuffers() {
    m_Framebuffers.resize(m_ImageViews.size());

    for (size_t i = 0; i < m_ImageViews.size(); i++) {
        vk::ImageView attachments[] = {
                m_ImageViews[i]
        };

        vk::FramebufferCreateInfo framebufferInfo = vk::FramebufferCreateInfo()
                .setRenderPass(m_RenderPass)
                .setAttachmentCount(1)
                .setPAttachments(attachments)
                .setWidth(m_Width)
                .setHeight(m_Height)
                .setLayers(1);

        vk::Result result = m_LogicalDevice.createFramebuffer(&framebufferInfo, nullptr, &m_Framebuffers[i]);
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error("Failed to create framebuffer! Error: " + vk::to_string(result));
        }
    }
}
