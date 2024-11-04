#include "Queue.hpp"

Graphics::Queue::QueueFamilyIndices Graphics::Queue::FindQueueFamilies(vk::PhysicalDevice gpu, vk::SurfaceKHR surface) {
    Graphics::Queue::QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    gpu.getQueueFamilyProperties(&queueFamilyCount, nullptr);

    std::vector<vk::QueueFamilyProperties> queueFamilies(queueFamilyCount);
    gpu.getQueueFamilyProperties(&queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies) {
        vk::Bool32 presentSupport = false;
        vk::Result result = gpu.getSurfaceSupportKHR(i, surface, &presentSupport);
        if (result != vk::Result::eSuccess)
        {
            throw std::runtime_error("Failed to physical device surface support! Error Code: " + vk::to_string(result));
        }

        if (queueFamily.queueCount > 0) {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                indices.graphicsFamily = i;
            }

            if (presentSupport) {
                indices.presentFamily = i;
            }
        }

        if (indices.IsComplete()) {
            break;
        }

        i++;
    }

    return indices;
}
