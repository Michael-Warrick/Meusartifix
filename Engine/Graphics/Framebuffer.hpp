#pragma once

#include <vulkan/vulkan.hpp>

namespace Graphics {
    class Framebuffer {
    public:
        Framebuffer(const vk::Device &logicalDevice, const std::vector<vk::ImageView> &imageViews, const vk::RenderPass &renderPass,
                    const uint32_t &width, const uint32_t &height);
        ~Framebuffer();

    private:
        void createFramebuffers();

        vk::Device m_LogicalDevice;
        std::vector<vk::ImageView> m_ImageViews;
        std::vector<vk::Framebuffer> m_Framebuffers;
        vk::RenderPass m_RenderPass;
        uint32_t m_Width;
        uint32_t m_Height;
    };
} // namespace Graphics