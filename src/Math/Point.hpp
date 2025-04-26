#pragma once

/**
 * @brief   Macro to define valid point-to-vector operations.
 *
 * Generates both binary and compound assignment versions of the operator.
 * Applies a vector operation element-wise to a point.
 *
 * @param   op  The operator to define (e.g., +, -)
 */
#define P_VEC_OP(op)                                                          \
    Point operator op(const Vec<N, T>& rhs) const                             \
    {                                                                         \
        Point res = *this;                                                    \
        std::transform(                                                       \
            this->_data.begin(),                                              \
            this->_data.end(),                                                \
            rhs.data().begin(),                                               \
            res.data().begin(),                                               \
            [](T a, T b) {                                                    \
                return a op b;                                                \
            }                                                                 \
        );                                                                    \
        return res;                                                           \
    }                                                                         \
                                                                              \
    Point& operator op##=(const Vec<N, T>& other)                             \
    {                                                                         \
        std::transform(                                                       \
            this->_data.begin(),                                              \
            this->_data.end(),                                                \
            other.data().begin(),                                             \
            this->_data.begin(),                                              \
            [](T a, T b) {                                                    \
                return a op b;                                                \
            }                                                                 \
        );                                                                    \
        return *this;                                                         \
    }

#include "Vec.hpp"

namespace raytracer::math
{

    /**
     * @brief   Represents a point in N-dimensional space.
     *
     * Inherits from Vec, but restricts certain operations that are not
     * semantically valid for points (e.g., adding two points or scaling a
     * point by a scalar).
     *
     * @tparam  N   Number of dimensions
     * @tparam  T   Type of coordinates (default: double)
     */
    template<size_t N, typename T = double>
    class Point
        : public Vec<N, T>
    {
    public:
        using Vec<N, T>::Vec;
        using Vec<N, T>::operator[];

        /**
         * @brief   Constructs a vector representing the direction from p1 to
         *          p2.
         *
         * @param   p1  Starting point
         * @param   p2  Ending point
         */
        static Vec<N, T> toVec(
            const Point& p1,
            const Point& p2
        )
        {
            Vec<N, T> res;

            std::transform(
                p2._data.begin(),
                p2._data.end(),
                p1._data.begin(),
                res.data().begin(),
                [](T b, T a) {
                    return b - a;
                }
            );
            return res;
        }

        /**
         * @brief   Deleted operator+: Adding two points is not allowed.
         */
        Point operator+(const Point&) const = delete;

        /**
         * @brief   Deleted operator+=: Adding another point to this point is
         *          not allowed.
         */
        Point& operator+=(const Point&) = delete;

        /**
         * @brief   Deleted operator-=: Subtracting a point directly from
         *          another point is disallowed.
         */
        Point& operator-=(const Point&) = delete;

        /**
         * @brief   Deleted operator*: Scaling a point by a scalar is not
         *          allowed.
         */
        Point operator*(T) const = delete;

        /**
         * @brief   Deleted operator*=: In-place scaling of a point is not
         *          allowed.
         */
        Point& operator*=(T) = delete;

        /**
         * @brief   Deleted operator/: Dividing a point by a scalar is not
         *          allowed.
         */
        Point operator/(T) const = delete;

        /**
         * @brief   Deleted operator/=: In-place division of a point by a
         *          scalar is not allowed.
         */
        Point& operator/=(T) = delete;

        P_VEC_OP(+);
        P_VEC_OP(-);

        /**
         * @brief   Computes a vector from the current point to another point.
         *
         * @param   other   The point to subtract from this point.
         * @return  Vec<N, T> A vector from `other` to `*this`.
         */
        Vec<N, T> operator-(const Point& other) const
        {
            Vec<N, T> res;
            std::transform(
                this->_data.begin(),
                this->_data.end(),
                other.data().begin(),
                res.data().begin(),
                [](T a, T b) {
                    return a - b;
                }
            );
            return res;
        }
    };

}
