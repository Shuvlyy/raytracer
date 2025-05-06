#pragma once

#include "Shape/Material/Material.hpp"
#include "Shape/Shape.hpp"

#define METAL "\"METAL\""

namespace raytracer::shape::material
{

    class Metal final
        : public Material
    {
    public:
        explicit Metal(
            const math::Color& color,
            const double fuzz
        ) : _color(color), _fuzz(fuzz) {}

        [[nodiscard]] bool scatter(
            const math::Ray& ray,
            HitResult& res,
            math::Color& attenuation,
            math::Ray& scattered
        ) const override
        {
            math::Vec<3> reflected = reflect(ray.direction, res.n);

            reflected = reflected.normalized() + this->_fuzz * math::Vec<3>::random();
            scattered = math::Ray(res.p, reflected);
            attenuation = this->_color;
            return true;
        }

    private:
        math::Color _color;
        double _fuzz; /// Fuzz is how blurry the metal texture will appear.
    };

}
