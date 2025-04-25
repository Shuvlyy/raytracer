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
    };

}
