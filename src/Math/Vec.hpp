#pragma once

/**
 * @brief   Macro to define element-wise vector-to-vector operators.
 *
 * Generates both binary and compound assignment versions of the operator.
 * Requires member `_data` to be a std::array of arithmetic types.
 *
 * @param   op  The operator to define (e.g., +, -, *, /)
 */
#define V_VEC_OP(op)                                                          \
    Vec operator op(const Vec& rhs) const                                     \
    {                                                                         \
        Vec res = *this;                                                      \
        std::transform(                                                       \
            this->_data.begin(),                                              \
            this->_data.end(),                                                \
            rhs._data.begin(),                                                \
            res._data.begin(),                                                \
            [](T a, T b) {                                                    \
                return a op b;                                                \
            }                                                                 \
        );                                                                    \
        return res;                                                           \
    }                                                                         \
                                                                              \
    Vec& operator op##=(const Vec& other)                                     \
    {                                                                         \
        std::transform(                                                       \
            this->_data.begin(),                                              \
            this->_data.end(),                                                \
            other._data.begin(),                                              \
            this->_data.begin(),                                              \
            [](T a, T b) {                                                    \
                return a op b;                                                \
            }                                                                 \
        );                                                                    \
        return *this;                                                         \
    }

/**
 * @brief   Macro to define element-wise scalar operations for vectors.
 *
 * Generates both binary and compound assignment versions of the operator with
 * a scalar.
 *
 * @param   op  The operator to define (e.g., +, -, *, /)
 */
#define V_SCALAR_OP(op)                                                       \
    template<typename S>                                                      \
    requires std::is_arithmetic_v<S>                                          \
    Vec operator op(S scalar) const {                                         \
        Vec res = *this;                                                      \
        std::for_each(                                                        \
            res._data.begin(),                                                \
            res._data.end(),                                                  \
            [=](T& x) {                                                       \
                x = x op static_cast<T>(scalar);                              \
            });                                                               \
        return res;                                                           \
    }                                                                         \
                                                                              \
    template<typename S>                                                      \
    requires std::is_arithmetic_v<S>                                          \
    Vec& operator op##=(S scalar) {                                           \
        std::for_each(                                                        \
            this->_data.begin(),                                              \
            this->_data.end(),                                                \
            [=](T& x) {                                                       \
                x = x op static_cast<T>(scalar);                              \
            });                                                               \
        return *this;                                                         \
    }                                                                         \

/**
 * @brief   Defines scalar-vector binary operators.
 *
 * Generates global operator overloads where the scalar is on the
 * left-hand side (lhs) and the vector is on the right-hand side (rhs).
 *
 * @param   op  The operator symbol (e.g., +, -, *, /).
 */
#define SCALAR_V_OP(op)                                                       \
    template<size_t N, typename T>                                            \
    Vec<N, T> operator op(T scalar, const Vec<N, T>& vec)                     \
    {                                                                         \
        return vec op scalar;                                                 \
    }

#include <array>
#include <algorithm>
#include <cmath>

#include "Random.hpp"

namespace raytracer::math
{

    /**
     * @brief   Represents an N-dimensional mathematical vector.
     *
     * Supports construction from values or two points, element access,
     * length computation, and vector arithmetic.
     *
     * @tparam  N   Number of dimensions
     * @tparam  T   Type of each component (default: double)
     */
    template<size_t N, typename T = double>
    requires std::is_arithmetic_v<T>
    class Vec
    {
    public:
        /**
         * @brief   Constructs a vector from a list of values.
         *
         * @tparam  Args    Parameter pack for values
         * @param   args    The values for each dimension
         *
         * @note    The number of arguments must exactly match the dimension N.
         */
        template<typename... Args>
        explicit Vec(Args... args)
            : _data{ static_cast<T>(args)... }
        {
            static_assert(
                sizeof...(Args) == N,
                "Number of arguments must match the vector dimensions."
            );
        }

        /**
         * @brief   Default constructor that initializes all components to
         *          zero.
         *
         * Constructs a vector where each component is set to the default value
         * of `T`, typically zero for arithmetic types like int or double.
         */
        Vec() { this->_data.fill(T{}); }

        Vec(const Vec& other)
            : _data(other._data)
        {}

        static Vec<N> random(double min = 0, double max = 1)
        {
            Vec<N> res;

            for (size_t i = 0; i < N; i++) {
                res.data()[i] = static_cast<T>(randomDouble(min, max));
            }
            return res;
        }

        /**
         * @brief   Computes the Euclidean length (magnitude) of the vector.
         *
         * @return  T Length of the vector
         */
        T length() const
        {
            T sum = T();

            for (size_t k = 0; k < N; k++) {
                sum += _data[k] * _data[k];
            }
            return std::sqrt(sum);
        }

        /**
         * @brief   Normalizes the vector and returns a new vector.
         *
         * @returns Vec A new normalized vector
         *
         * @note    If the vector has zero length, it returns a zero vector.
         */
        Vec normalized() const
        {
            T len = this->length();

            if (len == T(0)) {
                return Vec();
            }

            Vec res = *this;

            for (size_t i = 0; i < N; i++) {
                res[i] /= len;
            }
            return res;
        }


        /**
         * @brief   Normalizes the vector in-place.
         *
         * @return  Vec&    Reference to the normalized vector
         *
         * @note    If the vector has zero length, it will remain unchanged.
         */
        Vec& normalize()
        {
            T len = this->length();

            if (len == T(0)) {
                return *this;
            }

            for (size_t i = 0; i < N; i++) {
                this->_data[i] /= len;
            }

            return *this;
        }

        /**
         * @brief   Accesses the element at index i.
         *
         * @param   i   Index of the element
         * @return  T&  Reference to the element
         */
        T& operator[](size_t i) { return this->_data[i]; }

        /**
         * @brief   Accesses the element at index i (const).
         *
         * @param   i           Index of the element
         * @return  const T&    Constant reference to the element
         */
        const T& operator[](size_t i) const { return this->_data[i]; }

        V_VEC_OP(+);
        V_VEC_OP(-);
        V_VEC_OP(*);
        V_VEC_OP(/);

        V_SCALAR_OP(+);
        V_SCALAR_OP(-);
        V_SCALAR_OP(*);
        V_SCALAR_OP(/);

        Vec& operator=(const Vec& other)
        {
            if (this != &other) {
                this->_data = other._data;
            }
            return *this;
        }

        /**
         * @return  const std::array<T, N>& Const reference to the data array
         */
        const std::array<T, N>& data() const { return this->_data; }

        /**
         * @return  std::array<T, N>&   Reference to the data array
         */
        std::array<T, N>& data() { return this->_data; }

    protected:
        std::array<T, N> _data;
    };

    /**
     * @brief   Computes the dot product between two vectors.
     *
     * @tparam  N   Dimension of the vectors
     * @tparam  T   Type of the elements
     * @param   lhs Left-hand side vector
     * @param   rhs Right-hand side vector
     * @return  T   The dot product result
     */
    template<size_t N, typename T>
    T dot(
        const Vec<N, T>& lhs,
        const Vec<N, T>& rhs
    )
    {
        T result = T();

        for (size_t k = 0; k < N; k++) {
            result += lhs[k] * rhs[k];
        }
        return result;
    }

    /**
     * @brief   Reflects a vector across another vector.
     *
     * Computes the reflection of the vector `lhs` across the vector `rhs`.
     *
     * This operation assumes that `rhs` is a normalized vector.
     * If `rhs` is not normalized, the result will still be a valid reflection,
     * but it may not behave as expected in terms of geometric interpretation.
     *
     * @tparam  N           Dimension of the vectors (e.g., 2 for 2D, 3 for 3D)
     * @tparam  T           Type of the elements (e.g., `double`, `float`)
     * @param   lhs         The vector to be reflected
     * @param   rhs         The vector to reflect across
     * @return  Vec<N, T>   The reflected vector
     *
     * @note    If either vector has zero length, the behavior may not be
     *          well-defined.
     */
    template<size_t N, typename T>
    Vec<N, T> reflect(
        const Vec<N, T>& lhs,
        const Vec<N, T>& rhs
    )
    {
        return lhs - 2 * dot(lhs,rhs) * rhs;
    }

    SCALAR_V_OP(+);
    SCALAR_V_OP(-);
    SCALAR_V_OP(*);
    SCALAR_V_OP(/);

}
