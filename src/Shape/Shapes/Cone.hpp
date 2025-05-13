#pragma once

#include "Shape/Shape.hpp"
#define CONE "\"CONE\""

namespace raytracer::shape
{

    class Cone final
        : public Shape
    {
    public:

        Cone(
            const math::Point<3>& apex,
            const math::Vec<3>& axis,
            const double angle_rad,
            const std::shared_ptr<Material>& material
        ) : _apex(apex),
            _axis(normalize(axis)),
            _angle(angle_rad),
            _material(material)
        {
            const double clamped_angle = std::clamp(_angle, 0.0, M_PI_2);
            _cos2 = std::cos(clamped_angle) * std::cos(clamped_angle);
        }

        [[nodiscard]] bool hits(const math::Ray& ray, HitResult& res)
            const override
        {
            const math::Vec<3> co = ray.origin - _apex;
            const double dv = dot(ray.direction, _axis);
            const double cv = dot(co, _axis);

            const math::Vec<3> d_perp = ray.direction - dv * _axis;
            const math::Vec<3> co_perp = co - cv * _axis;

            const double a = dot(d_perp, d_perp) - (_cos2 * dv * dv);
            const double b = 2.0 * (dot(d_perp, co_perp) - (_cos2 * dv * cv));
            const double c = dot(co_perp, co_perp) - (_cos2 * cv * cv);

            const double delta = b * b - 4.0 * a * c;

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

            res.t = t;
            res.p = ray[res.t];

            const math::Vec<3> v = res.p - _apex;
            const double h = dot(v, _axis);
            const math::Point<3> c = _apex + h * _axis;
            const math::Vec<3> n = normalize(v - _axis * h * (1 + _cos2));

            res.n = n;
            res.material = this->_material;
            return true;
        }

    private:
        math::Point<3> _apex;
        math::Vec<3> _axis;
        double _angle;
        double _cos2;
        std::shared_ptr<Material> _material;
    };

}
