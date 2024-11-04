#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Graphics {
    class Surface {
    public:
        Surface(const vk::Instance &instance, GLFWwindow *window);
        ~Surface();

        vk::SurfaceKHR GetVulkanSurface();
    private:
        vk::Instance m_Instance;
        vk::SurfaceKHR m_Surface;
    };
} // namespace Graphics