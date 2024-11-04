#pragma once

#include <vulkan/vulkan.hpp>

#include <iostream>
#include <optional>

namespace Graphics {
    class Queue {
    public:
         struct QueueFamilyIndices {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool IsComplete() {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
         };

        static QueueFamilyIndices FindQueueFamilies(vk::PhysicalDevice gpu, vk::SurfaceKHR surface);
    private:
    };
} // namespace Graphics