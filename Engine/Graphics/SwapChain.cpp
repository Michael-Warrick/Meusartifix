#include "SwapChain.hpp"

Graphics::SwapChain::SwapChain(GLFWwindow *window, const vk::SurfaceKHR &surface,
                               const vk::PhysicalDevice &physicalDevice,
                               const vk::Device &logicalDevice) : m_Window(window), m_Surface(surface),
                                                                  m_PhysicalDevice(physicalDevice),
                                                                  m_LogicalDevice(logicalDevice) {
    createSwapChain();
}

Graphics::SwapChain::~SwapChain() {
    cleanupSwapChain();
}

void Graphics::SwapChain::Recreate() {
    recreateSwapChain();
}

void Graphics::SwapChain::createSwapChain() {
    SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_PhysicalDevice, m_Surface);

    vk::SurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(swapChainSupport.surfaceFormats);
    vk::PresentModeKHR presentMode = choosePresentMode(swapChainSupport.presentModes);
    vk::Extent2D extent = chooseExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    Queue::QueueFamilyIndices indices = Queue::FindQueueFamilies(m_PhysicalDevice, m_Surface);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    bool isConcurrentSharing = (indices.graphicsFamily != indices.presentFamily);

    vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR()
            .setSurface(m_Surface)
            .setMinImageCount(imageCount)
            .setImageFormat(surfaceFormat.format)
            .setImageColorSpace(surfaceFormat.colorSpace)
            .setImageExtent(extent)
            .setImageArrayLayers(1)
            .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
            .setImageSharingMode(isConcurrentSharing ? vk::SharingMode::eConcurrent : vk::SharingMode::eExclusive)
            .setQueueFamilyIndexCount(isConcurrentSharing ? 2 : 0)
            .setPQueueFamilyIndices(isConcurrentSharing ? queueFamilyIndices : nullptr)
            .setPreTransform(swapChainSupport.capabilities.currentTransform)
            .setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
            .setPresentMode(presentMode)
            .setClipped(vk::True);

    vk::Result result = m_LogicalDevice.createSwapchainKHR(&createInfo, nullptr, &m_SwapChain);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to create swap chain! Error Code: " + vk::to_string(result));
    }

    result = m_LogicalDevice.getSwapchainImagesKHR(m_SwapChain, &imageCount, nullptr);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to get swap chain images! Error Code: " + vk::to_string(result));
    }

    m_SwapChainImages.resize(imageCount);

    result = m_LogicalDevice.getSwapchainImagesKHR(m_SwapChain, &imageCount, m_SwapChainImages.data());
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to get swap chain images with data! Error Code: " + vk::to_string(result));
    }

    m_SwapChainImageFormat = surfaceFormat.format;
    m_SwapChainExtent = extent;
}

Graphics::SwapChain::SwapChainSupportDetails
Graphics::SwapChain::QuerySwapChainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
    SwapChainSupportDetails details;
    vk::Result result = physicalDevice.getSurfaceCapabilitiesKHR(surface, &details.capabilities);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to get swap chain surface capabilities! Error Code: " + vk::to_string(result));
    }

    uint32_t formatCount;
    result = physicalDevice.getSurfaceFormatsKHR(surface, &formatCount, nullptr);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to get swap chain surface formats! Error Code: " + vk::to_string(result));
    }

    if (formatCount != 0) {
        details.surfaceFormats.resize(formatCount);

        result = physicalDevice.getSurfaceFormatsKHR(surface, &formatCount, details.surfaceFormats.data());
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error(
                "Failed to get swap chain surface formats with data! Error Code: " + vk::to_string(result));
        }
    }

    uint32_t presentModeCount;
    result = physicalDevice.getSurfacePresentModesKHR(surface, &presentModeCount, nullptr);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to get swap chain present modes! Error Code: " + vk::to_string(result));
    }

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);

        result = physicalDevice.getSurfacePresentModesKHR(surface, &presentModeCount, details.presentModes.data());
        if (result != vk::Result::eSuccess) {
            throw std::runtime_error(
                "Failed to get swap chain present modes with data! Error Code: " + vk::to_string(result));
        }
    }

    return details;
}

vk::Format Graphics::SwapChain::GetImageFormat() const {
    return m_SwapChainImageFormat;
}

vk::SurfaceFormatKHR
Graphics::SwapChain::chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats) {
    for (const auto &availableFormat: availableFormats) {
        if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
            availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

vk::PresentModeKHR
Graphics::SwapChain::choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes) {
    for (const auto &availablePresentMode: availablePresentModes) {
        if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
            return availablePresentMode;
        }
    }

    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Graphics::SwapChain::chooseExtent(const vk::SurfaceCapabilitiesKHR &capabilities) const {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int width;
        int height;
        glfwGetFramebufferSize(m_Window, &width, &height);

        vk::Extent2D actualExtent = vk::Extent2D()
                .setWidth(static_cast<uint32_t>(width))
                .setHeight(static_cast<uint32_t>(height));

        actualExtent.setWidth(
            std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width));
        actualExtent.setHeight(
            std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                       capabilities.maxImageExtent.height));

        return actualExtent;
    }
}

void Graphics::SwapChain::createImageViews() {
    m_SwapChainImageViews.reserve(m_SwapChainImages.size());

    for (const auto &swapChainImage : m_SwapChainImages) {
        m_SwapChainImageViews.emplace_back(m_LogicalDevice, swapChainImage, m_SwapChainImageFormat, vk::ImageAspectFlagBits::eColor, 1);
    }
}

void Graphics::SwapChain::recreateSwapChain() {
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_Window, &width, &height);

    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(m_Window, &width, &height);
        glfwWaitEvents();
    }

    m_LogicalDevice.waitIdle();

    cleanupSwapChain();

    createSwapChain();
    createImageViews();
}

void Graphics::SwapChain::cleanupSwapChain() {
    for (auto &imageView : m_SwapChainImageViews) {
        imageView.CleanUp();
    }

    m_SwapChainImageViews.clear();  // Clears out any lingering references
    m_LogicalDevice.destroySwapchainKHR(m_SwapChain);
}

