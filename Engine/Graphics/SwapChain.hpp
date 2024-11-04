#pragma once

#include <iostream>
#include <limits>

#include <vulkan/vulkan.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "Queue.hpp"

namespace Graphics {
    class SwapChain {
    public:
        struct SwapChainSupportDetails {
            vk::SurfaceCapabilitiesKHR capabilities;
            std::vector<vk::SurfaceFormatKHR> surfaceFormats;
            std::vector<vk::PresentModeKHR> presentModes;
        };

        SwapChain(GLFWwindow *window, const vk::SurfaceKHR &surface, const vk::PhysicalDevice &physicalDevice, const vk::Device &logicalDevice);

        ~SwapChain();

        static SwapChainSupportDetails QuerySwapChainSupport(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
        void Recreate();

    private:
        void createSwapChain();
        static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR> &availableFormats);
        static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR> &availablePresentModes);
        vk::Extent2D chooseExtent(const vk::SurfaceCapabilitiesKHR &capabilities);


        void cleanupSwapChain();

        GLFWwindow *m_Window;
        vk::SurfaceKHR m_Surface;
        vk::Device m_LogicalDevice;
        vk::PhysicalDevice m_PhysicalDevice;
        vk::SwapchainKHR m_SwapChain;
        std::vector<vk::Image> m_SwapChainImages;
        vk::Format m_SwapChainImageFormat;
        vk::Extent2D m_SwapChainExtent;
        std::vector<vk::ImageView> m_SwapChainImageViews;
    };
} // namespace Graphics