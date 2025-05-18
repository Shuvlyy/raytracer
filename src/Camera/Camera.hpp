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
            math::Point<3> position;
            math::Vec<3> rotation;
            double fov{};
            double aperture{};
            double focalLength{};
        };

    }

    struct Camera final
    {
        math::Point<3> origin;
        math::Rect<3> screen;
        double lensRadius;
        uint32_t width, height;

        explicit Camera(const camera::Settings& settings);
        explicit Camera() :
            lensRadius(0)
        {}

        static Camera fromConfig(const yml::Yml& config);

        [[nodiscard]] math::Ray ray(double u, double v) const;
    };

}
