#pragma once

#include <iostream>
#include <memory>
#include <GLFW/glfw3.h>

#include "Instance.hpp"
#include "Surface.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

namespace Graphics
{
    class Renderer
    {
    public:
        explicit Renderer(GLFWwindow *window);

        void DrawFrame();

    private:
        GLFWwindow *m_Window;

        Instance m_Instance;
        Surface m_Surface;
        Device m_Device;
        SwapChain m_SwapChain;
    };
} // namespace Graphics
