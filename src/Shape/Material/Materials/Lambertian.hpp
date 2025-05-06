#pragma once

#include "Shape/Material/Material.hpp"
#include "Shape/Shape.hpp"

#define LAMBERTIAN "\"LAMBERTIAN\""

namespace raytracer::shape::material
{

    class Lambertian final
        : public Material
    {
    public:
        explicit Lambertian(const math::Color& color) : _color(color) {}

        [[nodiscard]] bool scatter(
            const math::Ray&,
            HitResult& res,
            math::Color& attenuation,
            math::Ray& scattered
        ) const override
        {
            auto dir = res.n + math::Vec<3>::random();

            scattered = math::Ray(res.p, dir);
            attenuation = this->_color;
            return true;
        }

    private:
        math::Color _color;
    };

}
