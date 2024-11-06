#pragma once

#include <vulkan/vulkan.hpp>

#include "SwapChain.hpp"

namespace Graphics {
    class Pipeline {
    public:
        Pipeline(const vk::Device &logicalDevice, const vk::Format &format);
        ~Pipeline();

    private:
        void createRenderPass();

        vk::Device m_LogicalDevice;
        vk::Format m_swapChainImageFormat;
    };
} // namespace Graphics