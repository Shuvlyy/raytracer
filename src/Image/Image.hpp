#pragma once

#include "Math/Color.hpp"
#include "Math/Vec.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace raytracer
{

    using PixelBuffer = std::vector<math::Color>;

    class Image
    {
    public:
        explicit Image(uint32_t width, uint32_t height);
        virtual ~Image() = default;

        [[nodiscard]] math::Color at(uint32_t x, uint32_t y) const;
        void setAt(uint32_t x, uint32_t y, math::Color color);

        virtual void save(const std::string& filepath) const = 0;

        [[nodiscard]] math::Vec<2, uint32_t> getDimensions() const;

    private:
        const uint32_t _width, _height;
        PixelBuffer _data;
    };

}
