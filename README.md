# Meusartifix [![Licence](https://img.shields.io/github/license/Michael-Warrick/Meusartifix?style=flat)](./LICENSE)

**Meusartifix** is a modular, Vulkan-based graphics and logic engine inspired by voxel games such as Minecraft, designed to allow users to configure various aspects of rendering, mechanics, and game logic. 
This project demonstrates state-of-the-art concepts in graphics programming, including command buffer management, pipelines, and user-editable configurations.

## Table of Contents
- [About](#about)
- [Project Structure](#project-structure)
- [Features](#features)
- [Installation](#getting-started)
- [Contributing](#contributing)
- [Disclaimer](#disclaimer)
- [License](#license)

## About
Meusartifix is built in C++ with RAII concepts and uses Vulkan for rendering, providing a modularized and extensible game engine framework.
It allows for high customization, with user-editable JSON configuration files, configurable rendering settings, and a flexible game systems.

## Project Structure
- **Assets:** Manages textures, models, shaders and other resources.
- **Engine:** Contains core game logic and Graphics directory.
- **Input:** Processes user input, such as keyboard and mouse interactions.
- **IO:** Manages file loading, saving, and other input/output operations.
- **Physics:** Implements physics calculations and collision detection.
- **config (planned):** Intended for user-editable JSON configuration files, like render settings and data.

## Features
- **Modularized Vulkan Components:** Each Vulkan concept is encapsulated in its own class, allowing for easy management and debugging.
- **JSON-Based Configuration:** All game settings, such as voxel data and render settings, are user-editable JSON files.
- **High-Performance Rendering:** Utilizes Vulkan to render complex scenes with custom shaders, supporting MSAA and efficient resource management.
- **Customizable Scripting API (In development):** C# scripting API similar to Unity, allowing users to define their own behaviors.

## Getting Started
### Prerequisites
[**CMake 3.28.1+**](https://cmake.org/download/#new_tab)\
[**Vulkan SDK 1.3+**](https://vulkan.lunarg.com/#new_tab)

### Installation

1. **Clone the Repository (with dependencies):**
```shell
$ git clone --recursive https://github.com/Michael-Warrick/Meusartifix.git
$ cd Meusartifix
```

2. **Build the Project:**
```shell
$ mkdir build && cd build
$ cmake ..
# Optionally build in Debug mode for testing
# cmake --build . --config Debug
$ cmake --build . --config Release
```

3. **Run**
```shell
$ ./Meusartifix
```

## Contributing
Contributions are welcome! Please open an issue or submit a pull request with improvements or bug fixes.

## Disclaimer
**Meusartifix** is an open-source, educational project developed solely for learning purposes and as a demonstration of graphics programming using Vulkan. This project is inspired by voxel-based games, but it is **not affiliated with, endorsed by, or associated with Mojang AB, Microsoft, or any of their products, including Minecraft**. All assets, code, and designs in Meusartifix are original and developed independently.

This project is provided freely as open-source software and is intended strictly for non-commercial use. It serves as an educational tool to allow developers, students, and enthusiasts to study and explore the implementation of graphics and game engine concepts.
- **No assets, code, or resources from Minecraft or Mojang are used in Meusartifix**.
- Meusartifix does not aim to replicate or replace any features of Minecraft and does not intend to infringe upon any existing intellectual property.
- This software is distributed under [License] (e.g., MIT), and users are free to modify and use the code in accordance with that license, with the understanding that it is for **educational and personal evaluation purposes only**.

If you have any concerns regarding the usage of this software, please feel free to reach out to the project maintainers.

## License
Distributed under the MIT License. See `LICENSE` for more information.