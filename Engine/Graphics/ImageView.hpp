#pragma once

#include <iostream>
#include <vulkan/vulkan.hpp>

namespace Graphics {
    class ImageView {
    public:
        ImageView();
        ~ImageView();

    private:
        void createImageViews();

        std::vector<vk::ImageView> imageViews;
    };
} // namespace Graphics