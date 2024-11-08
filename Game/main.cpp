#include "Core/Application.hpp"

int main() 
{
    try
    {
        Core::Application application;
        application.Run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}