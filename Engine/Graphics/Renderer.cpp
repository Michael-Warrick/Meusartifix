#include "Renderer.hpp"

Graphics::Renderer::Renderer(GLFWwindow *window) : m_Window(window), m_Surface(m_Instance.GetVulkanInstance(), window),
                                                   m_Device(m_Instance.GetVulkanInstance(),
                                                            m_Surface.GetVulkanSurface()),
                                                   m_SwapChain(window, m_Surface.GetVulkanSurface(),
                                                               m_Device.GetPhysicalVulkanDevice(),
                                                               m_Device.GetLogicalVulkanDevice()) {
}

void Graphics::Renderer::DrawFrame() {
    m_SwapChain.Recreate();
}