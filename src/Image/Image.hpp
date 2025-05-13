#pragma once

#include "Math/Color.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace raytracer
{

    using PixelBuffer = std::vector<math::Color>;

    /**
     * @brief   Represents a 2D image composed of RGB color pixels.
     *
     * The Image class provides a generic interface for pixel-based image
     * representation and manipulation. It stores pixels in a linear buffer
     * and supports querying, modifying, and saving image data.
     *
     * This class is abstract—derived classes must implement the `save()`
     * method.
     */
    class Image
    {
    public:
        /**
         * @brief   Constructs an image with the given width and height.
         *
         * All pixels are default-initialized to pure black (0, 0, 0).
         *
         * @param   width   Width of the image in pixels
         * @param   height  Height of the image in pixels
         */
        explicit Image(
            uint32_t width,
            uint32_t height
        );

        virtual ~Image() = default;

        /**
         * @brief   Returns the color of the pixel at (x, y).
         *
         * @param   x   The horizontal coordinate (0-based)
         * @param   y   The vertical coordinate (0-based)
         * @returns The color at the specified pixel
         */
        [[nodiscard]] math::Color at(uint32_t x, uint32_t y) const;

        /**
         * @brief   Sets the color of the pixel at (x, y).
         *
         * @param   x       Horizontal coordinate
         * @param   y       Vertical coordinate
         * @param   color   The color to assign to the specified pixel
         */
        void setAt(uint32_t x, uint32_t y, const math::Color& color);

        /**
         * @brief   Saves the image to the specified file.
         *
         * THIS IS A PURE VIRTUAL FUNCTION AND MUST BE IMPLEMENTED BY DERIVED
         * CLASSES.
         *
         * @param   filepath    Path to the output image file.
         */
        virtual void save(const std::string& filepath) const = 0;

        /**
         * @brief   Returns the dimensions of the image.
         *
         * @returns A 2D vector containing {width, height}
         */
        [[nodiscard]] math::Vec<2, uint32_t> getDimensions() const;


        /**
         * @returns Reference to the internal buffer containing all pixels
         */
        [[nodiscard]] const PixelBuffer& getData() const { return this->_data; }

        Image& operator+=(const Image& other);

    private:
        const uint32_t _width, _height;
        PixelBuffer _data;
    };

}
