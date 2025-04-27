#pragma once

#include <cstdint>

namespace raytracer::math
{

    struct Color final
    {
        uint8_t r, g, b, a;

        /**
         * @brief Constructs a Color object with specified RGBA values.
         *
         * @param   r   Red component (0–255)
         * @param   g   Green component (0–255)
         * @param   b   Blue component (0–255)
         * @param   a   Alpha (opacity) component (0–255), defaults to 255
         */
        Color(
            const uint8_t r,
            const uint8_t g,
            const uint8_t b,
            const uint8_t a = 255
        ) : r(r), g(g), b(b), a(a) {}

        /**
         * @brief   Constructs a default Color object (black, fully opaque).
         *
         * Sets r, g, b = 0, and a = 255.
         */
        Color() : r(0), g(0), b(0), a(255) {}

        /**
         * @brief   Adds two colors together component-wise.
         *
         * Alpha is preserved from the original (left-hand side) color.
         *
         * @param   color   The color to add.
         * @returns Color   The resulting color after addition.
         *
         * @note    No clamping is performed; values greater than 255 will wrap
         *          around.
         */
        Color operator+(const Color& color) const
        {
            return {
                static_cast<uint8_t>(r + color.r),
                static_cast<uint8_t>(g + color.g),
                static_cast<uint8_t>(b + color.b),
                a
            };
        }

    };

    /**
     * @brief   Multiplies a Color by a scalar value.
     *
     * Each RGB component is scaled by the given scalar, and the alpha channel
     * remains unchanged.
     *
     * @param   lhs     Scalar multiplier
     * @param   rhs     Color to be scaled
     * @returns Color   Resulting color after scaling
     *
     * @note    No clamping is performed; values greater than 255 will wrap
     *          around.
     */
    inline Color operator*(const double lhs, const Color& rhs)
    {
        return {
            static_cast<uint8_t>(rhs.r * lhs),
            static_cast<uint8_t>(rhs.g * lhs),
            static_cast<uint8_t>(rhs.b * lhs),
            rhs.a
        };
    }

}
