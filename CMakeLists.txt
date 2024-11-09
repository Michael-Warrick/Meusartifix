cmake_minimum_required(VERSION 3.28.1)
project(Meusartifix VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

add_subdirectory(Vendor/glfw)
add_subdirectory(Vendor/glm)

add_executable(${PROJECT_NAME}
        # Core
        ${CMAKE_SOURCE_DIR}/Engine/Core/Application.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Core/Window.cpp

        # Graphics
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/stb_image/stb_image_impl.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/Renderer.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/Instance.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/Surface.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/Device.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/Queue.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/SwapChain.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/ImageView.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/Pipeline.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/GraphicsPipeline.cpp
        ${CMAKE_SOURCE_DIR}/Engine/Graphics/Framebuffer.cpp

        # Input

        # IO
        ${CMAKE_SOURCE_DIR}/Engine/IO/File.cpp

        # Physics

        # Game
        ${CMAKE_SOURCE_DIR}/Game/main.cpp
)

add_subdirectory(Assets/Shaders)
add_dependencies(${PROJECT_NAME} Shaders)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<INSTALL_INTERFACE:Engine>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Engine>
        glfw
        glm
        Vulkan::Vulkan
)

target_link_libraries(${PROJECT_NAME}
        glfw
        glm
        Vulkan::Vulkan
)