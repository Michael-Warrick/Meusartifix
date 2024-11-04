#include "Surface.hpp"

Graphics::Surface::Surface(const vk::Instance &instance, GLFWwindow *window) : m_Instance(instance) {
    if (!window) {
        throw std::runtime_error("GLFW window pointer is null in Surface constructor!");
    }
    VkResult result = glfwCreateWindowSurface(
            m_Instance,
            window,
            nullptr,
            reinterpret_cast<VkSurfaceKHR *>(&m_Surface)
    );
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create surface!");
    }
}

Graphics::Surface::~Surface() {
    m_Instance.destroySurfaceKHR(m_Surface);
}

vk::SurfaceKHR Graphics::Surface::GetVulkanSurface() {
    return m_Surface;
}
