#pragma once

#include "Math/Vec.hpp"
#include "Math/Ray.hpp"

#include <optional>

namespace raytracer
{

    class Material;

    struct HitResult
    {
        double t = 0.0;
        math::Vec<3> p;
        Material* material = nullptr;
    };

    class Shape
    {
    public:
        virtual ~Shape() = default;

        [[nodiscard]] virtual bool hits(const math::Ray& ray, HitResult& res) const = 0;
    };

}
