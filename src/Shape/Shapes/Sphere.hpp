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
            const std::shared_ptr<Material>& material
        ) : _center(center), _radius(radius), _material(material) {}

        [[nodiscard]] bool hits(const math::Ray& ray, HitResult& res)
            const override
        {
            const math::Vec<3> r = ray.origin - this->_center;

            const double a = dot(ray.direction, ray.direction);
            const double b = dot(r, ray.direction);
            const double c = dot(r, r) - this->_radius * this->_radius;
            const double delta = b * b - a * c;

            if (delta < 0) {
                return false;
            }

            double x = (-b - std::sqrt(delta)) / a;

            if (x < RAY_T_MIN || x > ray.t_max) {
                x = (-b + std::sqrt(delta)) / a;
                if (x < RAY_T_MIN || x > ray.t_max) {
                    return false;
                }
                res.t = x;
            } else {
                res.t = x;
            }

            res.p = ray[res.t];
            res.n = (res.p - this->_center) / this->_radius;
            res.material = this->_material;
            return true;
        }

    private:
        math::Point<3> _center;
        double _radius;
        std::shared_ptr<Material> _material;
    };

}
