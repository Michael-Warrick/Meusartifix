#include "Application.hpp"

Core::Application::Application() 
{

}

Core::Application::~Application() 
{

}

void Core::Application::Run() 
{
    while (m_Window.IsOpen())
    {
        // Process Window Events
        m_Window.PollEvents();

        // Process Input State

        // Update Game State (Physics, Game Logic, AI...)

        // Render

        // Present
        m_Window.Present();
    }
    
}