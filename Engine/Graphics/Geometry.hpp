#pragma once

#include <vector>
#include "Vertex.hpp"

namespace Graphics {
    class Geometry {
    public:
        std::vector<Graphics::Vertex> GetTriangleVertices() {
            return triangleVertices;
        }

    private:
        const std::vector<Graphics::Vertex> triangleVertices = {
                {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        };
    };
} // namespace Graphics