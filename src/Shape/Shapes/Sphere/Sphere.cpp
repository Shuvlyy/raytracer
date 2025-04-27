#include "Sphere.hpp"

namespace raytracer::shape
{

    bool
    Sphere::hits
    (
        const math::Ray& ray,
        HitResult& res
    )
        const
    {
        const math::Vec<3> r = ray.origin - this->_center;

        const double a = dot(ray.direction, ray.direction);
        const double b = dot(r, ray.direction);
        const double c = dot(r, r) - this->_radius * this->_radius;
        const double delta = b * b - a * c;

        if (delta < 0) {
            return false;
        }

        const double x1 = (-b - std::sqrt(delta)) / a;
        const double x2 = (-b + std::sqrt(delta)) / a;

        if (x1 < RAY_T_MIN || x1 > ray.t_max) {
            if (x2 < RAY_T_MIN || x2 > ray.t_max) {
                return false;
            }
            res.t = x2;
        } else {
            res.t = x1;
        }

        res.p = ray[res.t];
        res.n = (res.p - this->_center) / this->_radius;
        res.material = this->_material;
        return true;
    }

}
