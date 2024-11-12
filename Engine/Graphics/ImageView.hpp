#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

namespace Graphics {
    class ImageView {
    public:
        ImageView(const vk::Device &logicalDevice, const vk::Image &image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels);
        ~ImageView();

        [[nodiscard]] vk::ImageView get() const ;

    private:
        void createImageView(vk::Image image, vk::Format format, vk::ImageAspectFlags aspectFlags, uint32_t mipLevels);

        vk::Device m_LogicalDevice;
        vk::ImageView m_ImageView;
    };
} // namespace Graphics