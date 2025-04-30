#pragma once

namespace raytracer::math
{

    /**
     * @brief   Represents a rectangle in N-dimensional space.
     *
     * A rectangle is defined by an origin point and two vectors that represent
     * the bottom side and left side.
     *
     * @tparam  N   The number of dimensions
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
         * @param   origin      The starting point (corner) of the rectangle
         * @param   bottomSide  Vector defining the bottom edge of the
         *                      rectangle
         * @param   leftSide    Vector defining the left edge of the rectangle
         */
        Rect(
            const Point<N, T>& origin,
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
         * @param   u   Scalar multiplier for the bottom side vector
         * @param   v   Scalar multiplier for the left side vector
         * @returns The computed point within the rectangle
         */
        [[nodiscard]] Point<N, T> at(const T u, const T v) const
        {
            return this->origin + this->bottomSide * u + this->leftSide * v;
        }
    };

}
