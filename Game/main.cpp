#include "Core/Application.hpp"

int main() 
{
    Core::Application application;
    try
    {
        application.Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}