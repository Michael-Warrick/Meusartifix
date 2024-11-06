#include "ImageView.hpp"


Graphics::ImageView::ImageView(const vk::Device &logicalDevice, const vk::Image &image,
                               const vk::Format format, const vk::ImageAspectFlags aspectFlags,
                               const uint32_t mipLevels) : m_LogicalDevice(logicalDevice) {
    createImageView(image, format, aspectFlags, mipLevels);
}

Graphics::ImageView::~ImageView() {
    // Only call CleanUp() here if absolutely necessary for unexpected cleanup.
    if (m_ImageView) {
        std::cerr << "Warning: ImageView destructor called without explicit cleanup. Consider handling in SwapChain.\n";
        CleanUp();
    }
}

vk::ImageView Graphics::ImageView::GetImageView() const {
    return m_ImageView;
}

void Graphics::ImageView::CleanUp() {
    if (m_ImageView) {
        m_LogicalDevice.destroyImageView(m_ImageView);
        m_ImageView = vk::ImageView();
    }
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