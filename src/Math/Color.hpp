#pragma once

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
    };

}
