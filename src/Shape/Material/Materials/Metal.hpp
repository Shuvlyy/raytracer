#pragma once

#include "Shape/Material/Material.hpp"
#include "Shape/Shape.hpp"

namespace raytracer::shape::material
{

    class Metal final
        : public Material
    {
    public:
        explicit Metal(const math::Color& color) : _color(color) {}

        [[nodiscard]] bool scatter(
            const math::Ray& ray,
            HitResult& res,
            math::Color& attenuation,
            math::Ray& scattered
        ) const override
        {
            math::Vec<3> reflected = reflect(ray.direction, res.n);

            scattered = math::Ray(res.p, reflected);
            attenuation = this->_color;
            return true;
        }

    private:
        math::Color _color;
    };

}
