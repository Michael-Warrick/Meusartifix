#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>

namespace Graphics {
    class Instance {
    public:
        Instance();

        ~Instance();

        [[nodiscard]] vk::Instance GetVulkanInstance() const;

    private:
        void createVulkanInstance();

        bool checkValidationLayerSupport();

        [[nodiscard]] std::vector<const char *> getRequiredExtensions() const;

        static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

        void setupDebugMessenger();

        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                            const VkDebugUtilsMessengerCallbackDataEXT *callbackData,
                                                            void *userData);

        static VkResult
        createDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *createInfo,
                                     const VkAllocationCallbacks *allocator,
                                     VkDebugUtilsMessengerEXT *debugMessenger);

        static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger,
                                                  const VkAllocationCallbacks *allocator);

        vk::Instance m_Instance;
        VkDebugUtilsMessengerEXT m_DebugMessenger = nullptr;
        const std::vector<const char *> m_ValidationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
        const bool m_ValidationLayersEnabled = false;
#else
        const bool m_ValidationLayersEnabled = true;
#endif

#ifdef __APPLE__
        const bool m_IsRunningOnMacOS = true;
#else
        const bool m_IsRunningOnMacOS = false;
#endif
    };

} // namespace Graphics
