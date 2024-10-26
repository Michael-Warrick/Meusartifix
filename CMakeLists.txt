cmake_minimum_required(VERSION 3.30.5)
project(Meusartifix VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

find_package(Vulkan REQUIRED)

add_subdirectory(Vendor/glfw)
add_subdirectory(Vendor/glm)

add_executable(${PROJECT_NAME}
    # Core
    ${CMAKE_SOURCE_DIR}/Engine/Core/Application.cpp
    ${CMAKE_SOURCE_DIR}/Engine/Core/Window.cpp

    # Game Main
    ${CMAKE_SOURCE_DIR}/Game/main.cpp
)

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