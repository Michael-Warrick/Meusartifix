#pragma once

#include <vulkan/vulkan.hpp>

#include "SwapChain.hpp"

namespace Graphics {
    class Pipeline {
    public:
        Pipeline(const vk::PhysicalDevice &physicalDevice, const vk::Device &logicalDevice, const vk::Format &format,
                 const vk::SampleCountFlagBits &msaaSamples);

        ~Pipeline();

    private:
        void createRenderPass();

        [[nodiscard]] vk::Format findDepthFormat() const;

        [[nodiscard]] vk::Format findSupportedFormat(const std::vector<vk::Format> &candidates, vk::ImageTiling tiling,
                                                     vk::FormatFeatureFlags features) const;

        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_LogicalDevice;
        vk::Format m_SwapChainImageFormat;
        vk::SampleCountFlagBits m_MSAASamples;
        vk::RenderPass m_RenderPass;
    };
} // namespace Graphics
