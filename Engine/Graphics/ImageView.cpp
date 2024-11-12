#include "ImageView.hpp"


Graphics::ImageView::ImageView(const vk::Device &logicalDevice, const vk::Image &image,
                               const vk::Format format, const vk::ImageAspectFlags aspectFlags,
                               const uint32_t mipLevels) : m_LogicalDevice(logicalDevice) {
    createImageView(image, format, aspectFlags, mipLevels);
}

Graphics::ImageView::~ImageView() {
    if (m_ImageView) {
        m_LogicalDevice.destroyImageView(m_ImageView);
        m_ImageView = vk::ImageView();
    }
}

vk::ImageView Graphics::ImageView::get() const {
    return m_ImageView;
}

void Graphics::ImageView::createImageView(const vk::Image image, const vk::Format format, const vk::ImageAspectFlags aspectFlags, const uint32_t mipLevels) {
    const vk::ImageViewCreateInfo viewInfo = vk::ImageViewCreateInfo()
        .setImage(image)
        .setViewType(vk::ImageViewType::e2D)
        .setFormat(format)
        .setSubresourceRange(
            vk::ImageSubresourceRange()
            .setAspectMask(aspectFlags)
            .setBaseMipLevel(0)
            .setLevelCount(mipLevels)
            .setBaseArrayLayer(0)
            .setLayerCount(1));

    vk::Result result = m_LogicalDevice.createImageView(&viewInfo, nullptr, &m_ImageView);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to create texture image view!" + vk::to_string(result));
    }
}