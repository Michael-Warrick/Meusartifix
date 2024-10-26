#pragma once

#include <iostream>
#include <GLFW/glfw3.h>

namespace Core
{
    class Window
    {
    public:
        Window();
        ~Window();

        void Present();
        void PollEvents();

        GLFWwindow *GetPlatformWindow();
        bool IsOpen();

    private:
        static void framebufferResizeCallback(GLFWwindow *window, int width, int height);

        GLFWwindow *m_Window = nullptr;

        uint16_t m_Width = 1280;
        uint16_t m_Height = 720;
        std::string m_Title = "Meusartifix 1.0.0";

        bool m_FramebufferResized = false;
    };
} // namespace Core
