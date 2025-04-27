#pragma once

#include "Shape/Shape.hpp"

namespace raytracer::shape
{

    class Sphere final
        : public Shape
    {
    public:
        Sphere(
            const math::Point<3>& center,
            const double radius,
            Material* material
        ) : _center(center), _radius(radius), _material(material) {}

        [[nodiscard]] bool hits(const math::Ray& ray, HitResult& res) const override;

    private:
        math::Point<3> _center;
        double _radius;
        Material* _material;
    };

}
