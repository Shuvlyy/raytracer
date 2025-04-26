#include "Image.hpp"

namespace raytracer
{

    Image::Image(
        const uint32_t width,
        const uint32_t height
    ) : _width(width), _height(height),
        _data(width * height)
    {}

    math::Color
    Image::at(
        const uint32_t x,
        const uint32_t y
    )
        const
    {
        return this->_data[y * this->_width + x];
    }

    void Image::setAt(
        const uint32_t x,
        const uint32_t y,
        const math::Color color
    )
    {
        this->_data[y * this->_width + x] = color;
    }

    math::Vec<2, uint32_t>
    Image::getDimensions()
        const
    {
        return math::Vec<2, uint32_t>(this->_width, this->_height);
    }

}
