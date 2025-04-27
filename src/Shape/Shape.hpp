#pragma once

#include <memory>

#include "Material/Material.hpp"
#include "Math/Vec.hpp"
#include "Math/Ray.hpp"

namespace raytracer
{

    struct HitResult
    {
        double t = 0.0;
        math::Point<3> p;
        math::Vec<3> n;
        std::shared_ptr<shape::Material> material;
    };

    class Shape
    {
    public:
        virtual ~Shape() = default;

        [[nodiscard]] virtual bool hits(
            const math::Ray& ray,
            HitResult& res
        ) const = 0;
    };

}
