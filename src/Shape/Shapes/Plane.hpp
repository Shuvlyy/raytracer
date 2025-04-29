#pragma once

#include "Shape/Shape.hpp"

namespace raytracer::shape
{

    class Plane final
        : public Shape
    {
    public:
        Plane(
            const math::Point<3>& point,
            const math::Vec<3>& axis,
            const std::shared_ptr<Material>& material
        ) : _point(point), _axis(axis), _material(material) {}

        [[nodiscard]] bool hits(const math::Ray& ray, HitResult& res)
            const override
        {
            const double d = dot(this->_axis, ray.direction);

            if (std::abs(d) < 1e-6) {
                return false;
            }

            const double t = dot(this->_point - ray.origin, this->_axis) / d;

            if (t < RAY_T_MIN || t > ray.t_max) {
                return false;
            }

            res.t = t;
            res.p = ray[res.t];
            res.n = this->_axis;
            res.material = this->_material;
            return true;
        }

    private:
        math::Point<3> _point;
        math::Vec<3> _axis;
        std::shared_ptr<Material> _material;
    };

}
