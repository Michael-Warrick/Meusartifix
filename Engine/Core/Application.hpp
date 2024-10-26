#include "Window.hpp"

namespace Core
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();

    private:
        Window m_Window;
    };
} // namespace Core