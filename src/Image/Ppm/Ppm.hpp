#pragma once

#define MAX_VAL 255

#include "Image/Image.hpp"

namespace raytracer::image
{

    class Ppm final
        : public Image
    {
    public:
        explicit Ppm(uint32_t width, uint32_t height);

        void save(const std::string &filepath) const override;
    };

}
