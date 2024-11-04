#include "Instance.hpp"

Graphics::Instance::Instance() {
    createVulkanInstance();
    setupDebugMessenger();
}

Graphics::Instance::~Instance() {
    if (m_ValidationLayersEnabled) {
        destroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
    }

    m_Instance.destroy();
}

vk::Instance Graphics::Instance::GetVulkanInstance() const {
    return m_Instance;
}

void Graphics::Instance::createVulkanInstance() {
    if (m_ValidationLayersEnabled && !checkValidationLayerSupport()) {
        throw std::runtime_error("Requested validation layers unavailable!");
    }

    vk::ApplicationInfo applicationInfo = vk::ApplicationInfo()
            .setPApplicationName("Meusartifix")
            .setApplicationVersion(vk::makeApiVersion(
                    0,
                    1,
                    0,
                    0)
            )
            .setPEngineName("COMRADE")
            .setEngineVersion(vk::makeApiVersion(0, 1, 0, 0))
            .setApiVersion(vk::ApiVersion13);

    vk::DebugUtilsMessengerCreateInfoEXT debugCreateInfo = vk::DebugUtilsMessengerCreateInfoEXT();
    if (m_ValidationLayersEnabled) {
        populateDebugMessengerCreateInfo(debugCreateInfo);
    }

    auto requiredExtensions = getRequiredExtensions();
    vk::InstanceCreateInfo createInfo = vk::InstanceCreateInfo()
            .setFlags(m_IsRunningOnMacOS ? vk::InstanceCreateFlags(vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR) : vk::InstanceCreateFlags())
            .setPApplicationInfo(&applicationInfo)
            .setEnabledExtensionCount(static_cast<uint32_t>(requiredExtensions.size()))
            .setPpEnabledExtensionNames(requiredExtensions.data())
            .setEnabledLayerCount(
                    m_ValidationLayersEnabled ? static_cast<uint32_t>(m_ValidationLayers.size()) : 0)
            .setPpEnabledLayerNames(m_ValidationLayersEnabled ? m_ValidationLayers.data() : nullptr)
            .setPNext(
                    m_ValidationLayersEnabled ? (vk::DebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo : nullptr);

    try {
        m_Instance = vk::createInstance(createInfo);
    }
    catch (const vk::SystemError &err) {
        throw std::runtime_error("Failed to create Vulkan instance: " + std::string(err.what()));
    }
}

bool Graphics::Instance::checkValidationLayerSupport() {
    uint32_t layerCount;
    vk::Result result = vk::enumerateInstanceLayerProperties(&layerCount, nullptr);
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to enumerate instance layer count. Error code: " + vk::to_string(result));
    }

    std::vector<vk::LayerProperties> availableLayers(layerCount);

    result = vk::enumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    if (result != vk::Result::eSuccess) {
        throw std::runtime_error("Failed to enumerate instance available layers. Error code: " + vk::to_string(result));
    }

    for (const char *layerName : m_ValidationLayers) {
        bool layerWasFound = false;
        for (const auto &layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerWasFound = true;
                break;
            }
        }

        if (!layerWasFound) {
            return false;
        }
    }

    return true;
}

std::vector<const char *> Graphics::Instance::getRequiredExtensions() const {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (m_IsRunningOnMacOS) {
        // Adding the portability extension (for MoltenVK driver compatibility issue) + Setting flag
        extensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    }

    if (m_ValidationLayersEnabled) {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
}

void Graphics::Instance::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
    createInfo = vk::DebugUtilsMessengerCreateInfoEXT()
            .setMessageSeverity(
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
                    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
            )
            .setMessageType(
                    vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
                    vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
                    vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
            )
            .setPfnUserCallback(debugCallback);
}

void Graphics::Instance::setupDebugMessenger() {
    if (!m_ValidationLayersEnabled) {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    if (createDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("Failed to set up debug messenger!");
    }
}

VkResult Graphics::Instance::createDebugUtilsMessengerEXT(VkInstance instance,
                                                          const VkDebugUtilsMessengerCreateInfoEXT *createInfo,
                                                          const VkAllocationCallbacks *allocator,
                                                          VkDebugUtilsMessengerEXT *debugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, createInfo, allocator, debugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Graphics::Instance::destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* allocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, allocator);
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL Graphics::Instance::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                                 const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
                                                                 void *userData) {
    std::cerr << "[VALIDATION LAYER]: " << callbackData->pMessage << std::endl;
    return VK_FALSE;
}