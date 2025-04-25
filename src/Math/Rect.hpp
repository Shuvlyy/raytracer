#pragma once

namespace raytracer::math
{

    /**
     * @brief   Represents a rectangle in N-dimensional space.
     *
     * A rectangle is defined by an origin point and two vectors that represent
     * the bottom and left sides.
     * The rectangle is formed in the plane spanned by these two vectors,
     * allowing it to exist in any dimension.
     *
     * @tparam  N   The number of dimensions (e.g., 3 for 3D, 2 for 2D, etc.)
     * @tparam  T   The type of coordinates (default: double)
     */
    template<size_t N, typename T = double>
    struct Rect final
    {
        Point<N, T> origin;
        Vec<N, T> bottomSide, leftSide;

        /**
         * @brief   Constructs a rectangle with the given origin and side
         *          vectors.
         *
         * The rectangle is defined by an origin point and two vectors
         * representing the bottom and left sides.
         *
         * @param   origin      The starting point (corner) of the rectangle.
         * @param   bottomSide  Vector defining the bottom edge of the
         *                      rectangle.
         * @param   leftSide    Vector defining the left edge of the rectangle.
         */
        Rect(const Point<N, T>& origin,
             const Vec<N, T>& bottomSide,
             const Vec<N, T>& leftSide
        ) : origin(origin), bottomSide(bottomSide), leftSide(leftSide) {}

        /**
         * @brief   Default constructor for an uninitialized rectangle.
         *
         * Initializes the rectangle with default values.
         */
        Rect() = default;

        /**
         * @brief   Computes a point within the rectangle given two parameters.
         *
         * This method computes a point within the rectangle using two
         * parameters:
         * - `u`: A scalar multiplier for the bottom side vector.
         * - `v`: A scalar multiplier for the left side vector.
         *
         * The computed point is given by the formula:
         * `origin + bottomSide * u + leftSide * v`
         *
         * @param   u           Scalar multiplier for the bottom side vector.
         * @param   v           Scalar multiplier for the left side vector.
         * @return  Point<N, T> The computed point within the rectangle.
         */
        [[nodiscard]] Point<N, T> at(const T u, const T v) const
        {
            return this->origin + this->bottomSide * u + this->leftSide * v;
        }
    };

}
