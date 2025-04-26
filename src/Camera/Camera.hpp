#pragma once

#include "Math/Point.hpp"
#include "Math/Ray.hpp"
#include "Math/Rect.hpp"
#include "yml/Yml.hpp"

#include <cstdint>

namespace raytracer
{

    namespace camera
    {

        struct Settings
        {
            uint32_t width{}, height{};
            math::Vec<3> position, rotation;
            double fov{};
            double aperture{};
        };

    }

    struct Camera final
    {
        math::Point<3> origin;
        math::Rect<3> screen;

        explicit Camera(const camera::Settings& settings);

        static Camera fromConfig(const yml::Yml& config);

        [[nodiscard]] math::Ray ray(double u, double v) const;
    };

}
