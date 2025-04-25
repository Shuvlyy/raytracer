#pragma once

#define RAY_T_MIN   0.0001
#define RAY_T_MAX   1.0e30

#include "Math/Vec.hpp"
#include "Math/Point.hpp"

namespace raytracer::math
{

    /**
     * @brief   Represents a 3D ray used in ray tracing computations.
     *
     * A ray is defined by an origin point, a direction vector, and a maximum
     * travel distance (t_max).
     */
    struct Ray final
    {
        Point<3> origin;
        Vec<3> direction;
        double t_max;

        /**
         * @brief   Constructs a ray from an origin, direction, and maximum
         *          distance.
         *
         * @param   origin      The starting point of the ray.
         * @param   direction   The direction in which the ray travels.
         * @param   t_max       The maximum value of the parameter t (upper
         *                      bound for ray intersection).
         */
        explicit Ray(
            const Point<3>& origin,
            const Vec<3>& direction,
            const double t_max = RAY_T_MAX
        ) : origin(origin), direction(direction), t_max(t_max) {}

        /**
         * @brief   Evaluates the ray at a given parameter t.
         *
         * Computes the point along the ray at distance `t` from the origin:
         * `origin + direction * t`
         *
         * @param   t           The parameter along the ray.
         * @return  Point<3>    The point at parameter t along the ray.
         */
        Point<3> operator[](const double t) const
        {
            return this->origin + this->direction * t;
        }
    };

}
