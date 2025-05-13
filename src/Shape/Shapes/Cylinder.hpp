#pragma once

#include "Shape/Shape.hpp"
#define CYLINDER "\"CYLINDER\""

namespace raytracer::shape
{

    class Cylinder final
        : public Shape
    {
    public:

        Cylinder(
            const math::Point<3>& baseCenter,
            const math::Vec<3>& axis,
            double radius,
            double height,
            const std::shared_ptr<Material>& material
        ) : _base(baseCenter),
            _axis(normalize(axis)),
            _radius(radius),
            _height(height),
            _material(material)
        {
            _radius2 = radius * radius;
        }

        [[nodiscard]] bool hits(const math::Ray& ray, HitResult& res)
            const override
        {
            const math::Vec<3> deltaP = ray.origin - _base;

            const double d_dot_a = dot(ray.direction, _axis);
            const double dp_dot_a = dot(deltaP, _axis);

            const math::Vec<3> d_perp = ray.direction - d_dot_a * _axis;
            const math::Vec<3> dp_perp = deltaP - dp_dot_a * _axis;

            const double a = dot(d_perp, d_perp);
            const double b = 2 * dot(d_perp, dp_perp);
            const double c = dot(dp_perp, dp_perp) - _radius2;

            const double delta = b * b - 4 * a * c;

            if (delta < 0.0) {
                return false;
            }

            double t = (-b - std::sqrt(delta)) / (2.0 * a);

            if (t < RAY_T_MIN || t > ray.t_max) {
                t = (-b + std::sqrt(delta)) / (2.0 * a);
                if (t < RAY_T_MIN || t > ray.t_max) {
                    return false;
                }
            }

            const math::Point<3> hitPoint = ray[t];
            const double projHeight = dot(hitPoint - _base, _axis);

            if (projHeight < 0 || projHeight > _height) {
                return false;
            }

            res.t = t;
            res.p = hitPoint;

            const math::Point<3> c = _base + projHeight * _axis;
            res.n = normalize(hitPoint - c);
            res.material = _material;

            return true;
        }

    private:
        math::Point<3> _base;
        math::Vec<3> _axis;
        double _radius;
        double _height;
        double _radius2;
        std::shared_ptr<Material> this->_material;
    };

}
