#pragma once

#include "Vec.hpp"

#include <cstdint>

namespace raytracer::math
{

    /**
     * @brief   Represents an RGB color using a 3-dimensional vector.
     *
     * This type alias defines a color as a Vec<3>, where each component
     * corresponds to the red, green, and blue channels respectively.
     * It can be used for color operations such as blending, scaling, and
     * clamping.
     *
     * @note The component values can represent normalized (0.0–1.0) or 8-bit
     *       (0–255) color channels, depending on context.
     */
    using Color = Vec<3>;

}
