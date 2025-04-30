#pragma once

#include "Math/Ray.hpp"
#include "Math/Color.hpp"

namespace raytracer
{
    struct HitResult;
}

namespace raytracer::shape
{

    class Material
    {
    public:
        virtual ~Material() = default;

        [[nodiscard]] virtual bool scatter(
            const math::Ray& ray,
            HitResult& res,
            math::Color& attenuation,
            math::Ray& scattered
        ) const = 0;

        [[nodiscard]] virtual math::Color emitted(
            [[maybe_unused]] const math::Ray& ray,
            [[maybe_unused]] HitResult& res
        ) const { return {}; }
    };

}
