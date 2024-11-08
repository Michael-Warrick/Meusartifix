#include "Device.hpp"

Graphics::Device::Device(const vk::Instance &instance, const vk::SurfaceKHR &surface) : m_Instance(instance),
                                                                                        m_Surface(surface) {
    pickPhysicalDevice();
    createLogicalDevice();
}

Graphics::Device::~Device() {
    m_LogicalDevice.destroy();
}

vk::PhysicalDevice Graphics::Device::GetPhysicalVulkanDevice() const {
    return m_PhysicalDevice;
}

vk::Device Graphics::Device::GetLogicalVulkanDevice() const {
    return m_LogicalDevice;
}

vk::SampleCountFlagBits Graphics::Device::GetMSAASamples() const {
    return m_MSAASamples;
}


void Graphics::Device::pickPhysicalDevice() {
    uint32_t gpuCount = 0;
    vk::Result result = m_Instance.enumeratePhysicalDevices(&gpuCount, nullptr);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to enumerate physical devices. Error code: " + vk::to_string(result));
    }

    if (gpuCount == 0) {
        throw std::runtime_error("Failed to find GPUs with Vulkan support!");
    }

    std::vector<vk::PhysicalDevice> gpus(gpuCount);
    result = m_Instance.enumeratePhysicalDevices(&gpuCount, gpus.data());
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to enumerate GPUs with selected data. Error code: " + vk::to_string(result));
    }

    for (const auto &gpu: gpus) {
        if (isPhysicalDeviceSuitable(gpu)) {
            m_PhysicalDevice = gpu;
            m_MSAASamples = getMaxUsableSamples();

            break;
        }
    }

    if (!m_PhysicalDevice) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}

bool Graphics::Device::isPhysicalDeviceSuitable(vk::PhysicalDevice physicalDevice) {
    Queue::QueueFamilyIndices indices = Queue::FindQueueFamilies(physicalDevice, m_Surface);
    bool extensionsAreSupported = checkPhysicalDeviceExtensionSupport(physicalDevice);

    bool swapChainIsAdequate = false;
    if (extensionsAreSupported) {
        SwapChain::SwapChainSupportDetails swapChainSupport = SwapChain::QuerySwapChainSupport(physicalDevice, m_Surface);
        swapChainIsAdequate = !swapChainSupport.surfaceFormats.empty() && !swapChainSupport.presentModes.empty();
    }

    vk::PhysicalDeviceFeatures supportedFeatures = physicalDevice.getFeatures();

    return indices.IsComplete() && extensionsAreSupported && swapChainIsAdequate && supportedFeatures.samplerAnisotropy;
}

bool Graphics::Device::checkPhysicalDeviceExtensionSupport(vk::PhysicalDevice physicalDevice) {
    uint32_t extensionCount = 0;
    vk::Result result = physicalDevice.enumerateDeviceExtensionProperties(nullptr, &extensionCount, nullptr);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error(
                "Failed to check physical device extension support. Error code: " + vk::to_string(result));
    }

    std::vector<vk::ExtensionProperties> availableExtensions(extensionCount);
    result = physicalDevice.enumerateDeviceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error(
                "Failed to check physical device extension support with data. Error code: " + vk::to_string(result));
    }

    std::set < std::string > requiredExtensions(m_PhysicalDeviceExtensions.begin(), m_PhysicalDeviceExtensions.end());
    for (const auto &extension: availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

vk::SampleCountFlagBits Graphics::Device::getMaxUsableSamples() const {
    vk::PhysicalDeviceProperties physicalDeviceProperties;
    m_PhysicalDevice.getProperties(&physicalDeviceProperties);

    vk::SampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts &
                                  physicalDeviceProperties.limits.framebufferDepthSampleCounts;
    if (counts & vk::SampleCountFlagBits::e64) { return vk::SampleCountFlagBits::e64; }
    if (counts & vk::SampleCountFlagBits::e32) { return vk::SampleCountFlagBits::e32; }
    if (counts & vk::SampleCountFlagBits::e16) { return vk::SampleCountFlagBits::e16; }
    if (counts & vk::SampleCountFlagBits::e8) { return vk::SampleCountFlagBits::e8; }
    if (counts & vk::SampleCountFlagBits::e4) { return vk::SampleCountFlagBits::e4; }
    if (counts & vk::SampleCountFlagBits::e2) { return vk::SampleCountFlagBits::e2; }

    return vk::SampleCountFlagBits::e1;
}

void Graphics::Device::createLogicalDevice() {
    Queue::QueueFamilyIndices indices = Queue::FindQueueFamilies(m_PhysicalDevice, m_Surface);

    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        vk::DeviceQueueCreateInfo queueCreateInfo = vk::DeviceQueueCreateInfo()
                .setQueueFamilyIndex(queueFamily)
                .setQueueCount(1)
                .setPQueuePriorities(&queuePriority);

        queueCreateInfos.push_back(queueCreateInfo);
    }

    vk::PhysicalDeviceFeatures physicalDeviceFeatures;
    physicalDeviceFeatures.samplerAnisotropy = vk::True;
    physicalDeviceFeatures.sampleRateShading = vk::True;

    vk::DeviceCreateInfo createInfo = vk::DeviceCreateInfo()
            .setQueueCreateInfoCount(static_cast<uint32_t>(queueCreateInfos.size()))
            .setPQueueCreateInfos(queueCreateInfos.data())
            .setPEnabledFeatures(&physicalDeviceFeatures)
            .setEnabledExtensionCount(static_cast<uint32_t>(m_PhysicalDeviceExtensions.size()))
            .setPpEnabledExtensionNames(m_PhysicalDeviceExtensions.data())
            .setEnabledLayerCount(m_ValidationLayersEnabled ? static_cast<uint32_t>(m_ValidationLayers.size()) : 0)
            .setPpEnabledLayerNames(m_ValidationLayersEnabled ? m_ValidationLayers.data() : nullptr);

    vk::Result result = m_PhysicalDevice.createDevice(&createInfo, nullptr, &m_LogicalDevice);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to create logical device! Error code: " + vk::to_string(result));
    }

    m_LogicalDevice.getQueue(indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
    m_LogicalDevice.getQueue(indices.presentFamily.value(), 0, &m_PresentQueue);
}

