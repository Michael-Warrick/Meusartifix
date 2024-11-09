#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

#include "../IO/File.hpp"
#include "Vertex.hpp"
#include "Geometry.hpp"

namespace Graphics {
    class GraphicsPipeline {
    public:
        GraphicsPipeline(const vk::Device &logicalDevice, const vk::SampleCountFlagBits &msaaSamples);
        ~GraphicsPipeline();

    private:
        void createGraphicsPipeline(const std::string &vertexShaderFilePath, const std::string &fragmentShaderFilePath);
        vk::ShaderModule createShaderModule(const std::vector<char> &code);

        vk::Device m_LogicalDevice;
        vk::SampleCountFlagBits m_MSAASamples;
    };
} // namespace Graphics