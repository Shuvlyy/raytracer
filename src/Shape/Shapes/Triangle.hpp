#pragma once

#include "Math/Mat.hpp"
#include "Math/Point.hpp"
#include "Shape/Shape.hpp"
#define TRIANGLE "\"TRIANGLE\""
#define OBJ "\"OBJ\""

namespace raytracer::shape
{

    class Triangle final
        : public Shape
    {
    public:

        Triangle(
            const math::Point<3> &p1,
            const math::Point<3> &p2,
            const math::Point<3> &p3,
            const math::Vec<3> &axis,
            const std::shared_ptr<Material>& material
        ) : _p1(p1), _p2(p2), _p3(p3), _axis(axis), _material(material) {}

        [[nodiscard]] bool hits(const math::Ray& ray, HitResult& res)
            const override
        {
            const math::Vec<3> edge1 = this->_p2 - this->_p1;
            const math::Vec<3> edge2 = this->_p3 - this->_p1;

            const math::Vec<3> h = cross(ray.direction, edge2);
            const double a = dot(edge1, h);

            if (std::fabs(a) < ALMOST_ZERO)
                return false;

            const double f = 1.0 / a;
            const math::Vec<3> s = ray.origin - this->_p1;
            const double u = f * dot(s, h);

            if (u < 0.0 || u > 1.0)
                return false;

            const math::Vec<3> q = cross(s, edge1);
            const double v = f * dot(ray.direction, q);

            if (v < 0.0 || (u + v) > 1.0)
                return false;

            const double t = dot(edge2, q);

            if (t < RAY_T_MIN || t > ray.t_max)
                return false;

            res.t = t;
            res.p = ray[res.t];
            res.n = this->_axis;
            res.material = _material;
            return true;
        }

    private:
        math::Point<3> _p1;
        math::Point<3> _p2;
        math::Point<3> _p3;
        math::Vec<3> _axis;
        std::shared_ptr<Material> _material;
    };

}
