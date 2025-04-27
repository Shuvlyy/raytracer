#pragma once

#include "Vec.hpp"

#include <cstdint>

namespace raytracer::math
{

    struct Color final
        : Vec<3, uint8_t>
    {

        using Vec::Vec;

        /**
         * @brief Constructs a Color object with specified RGBA values.
         *
         * @param   r   Red component (0–255)
         * @param   g   Green component (0–255)
         * @param   b   Blue component (0–255)
         */
        Color(
            const uint8_t r,
            const uint8_t g,
            const uint8_t b
        ) : Vec(r, g, b) {}

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
                static_cast<uint8_t>(this->r() + color.r()),
                static_cast<uint8_t>(this->g() + color.g()),
                static_cast<uint8_t>(this->b() + color.b())
            };
        }

        [[nodiscard]] uint8_t r() const { return this->data()[0]; }
        [[nodiscard]] uint8_t g() const { return this->data()[1]; }
        [[nodiscard]] uint8_t b() const { return this->data()[2]; }
        [[nodiscard]] uint8_t a() const { return this->data()[3]; }

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
            static_cast<uint8_t>(rhs.r() * lhs),
            static_cast<uint8_t>(rhs.g() * lhs),
            static_cast<uint8_t>(rhs.b() * lhs)
        };
    }

}
