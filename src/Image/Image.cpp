#include "Image.hpp"

namespace raytracer
{

    Image::Image
    (
        const uint32_t width,
        const uint32_t height
    ) : _width(width), _height(height),
        _data(width * height) {}

    math::Color
    Image::at
    (
        const uint32_t x,
        const uint32_t y
    )
        const
    {
        const uint64_t rawCoordinates = y * this->_width + x;

        if (rawCoordinates > this->_data.size()) {
            throw; // TODO: Throw exception
        }
        return this->_data[rawCoordinates];
    }

    void Image::setAt
    (
        const uint32_t x,
        const uint32_t y,
        const math::Color& color
    )
    {
        const uint64_t rawCoordinates = y * this->_width + x;

        if (rawCoordinates > this->_data.size()) {
            throw; // TODO: Throw exception
        }
        this->_data[rawCoordinates] = color;
    }

    math::Vec<2, uint32_t>
    Image::getDimensions()
        const
    {
        return math::Vec<2, uint32_t>(this->_width, this->_height);
    }

    Image &
    Image::operator+=
    (
        const Image &other
    )
    {
        std::transform(
            this->_data.begin(),
            this->_data.end(),
            other._data.begin(),
            this->_data.begin(),
            [](const math::Color& a, const math::Color& b) {
                return a + b;
            }
        );
        return *this;
    }

}
