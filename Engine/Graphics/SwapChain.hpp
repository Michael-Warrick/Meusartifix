#pragma once

#include <iostream>
#include <limits>

#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#include "Queue.hpp"
#include "ImageView.hpp"
#include "Framebuffer.hpp"

namespace Graphics {
    class SwapChain {
    public:
        struct SwapChainSupportDetails {
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> surfaceFormats;
            std::vector<vk::PresentModeKHR> presentModes;
        };

        SwapChain(GLFWwindow *window, const vk::SurfaceKHR &surface, const vk::PhysicalDevice &physicalDevice,
                  const vk::Device &logicalDevice);

        ~SwapChain();

        void CreateFramebuffers(vk::RenderPass renderPass);

        void Recreate(vk::RenderPass renderPass);

        static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

        [[nodiscard]] vk::Format imageFormat() const;

        [[nodiscard]] std::vector<vk::ImageView> imageViews() const;

        [[nodiscard]] uint32_t extentWidth() const;

        [[nodiscard]] uint32_t extentHeight() const;

    private:
        void createSwapChain();

        static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);

        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);

        [[nodiscard]] vk::Extent2D chooseExtent(const vk::SurfaceCapabilitiesKHR &capabilities) const;

        void createImageViews();

        void createSwapChainFramebuffers(vk::RenderPass renderPass);

        void recreateSwapChain(vk::RenderPass renderPass);

        void cleanupSwapChain();

        GLFWwindow *m_Window;

        vk::SurfaceKHR m_Surface;
        vk::PhysicalDevice m_PhysicalDevice;
        vk::Device m_LogicalDevice;

        vk::SwapchainKHR m_SwapChain;
        std::vector<vk::Image> m_SwapChainImages;
        vk::Format m_SwapChainImageFormat;
        vk::Extent2D m_SwapChainExtent;
        std::vector<vk::ImageView> m_SwapChainImageViews;
        std::optional<Framebuffer> m_Framebuffer;
    };
} // namespace Graphics