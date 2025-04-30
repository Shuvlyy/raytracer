#pragma once

/**
 * @brief   Macro to define element-wise vector-to-vector operators.
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
     * Provides a variety of operations for working with vectors in arbitrary
     * dimensions, including arithmetic, normalization, clamping, and random
     * generation. It supports both vector-to-vector and vector-to-scalar
     * operations through macros.
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
         * @note    The number of arguments must exactly match the dimension N
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
         */
        Vec() { this->_data.fill(T{}); }

        Vec(const Vec& other) : _data(other._data) {}

        /**
         * @brief   Generates a random vector with components in the given
         *          range [min, max).
         *
         * @param   min Minimum value for each component
         * @param   max Maximum value for each component
         * @returns A randomly generated vector of dimension N
         */
        static Vec<N> random(
            const double min = 0,
            const double max = 1
        )
        {
            Vec<N> res;

            for (size_t i = 0; i < N; i++) {
                res.data()[i] = static_cast<T>(randomDouble(min, max));
            }
            return res;
        }

        /**
         * @brief   Generates a random 3D unit vector.
         *
         * Samples points in a unit sphere until one falls inside, then
         * normalizes it. This ensures uniform distribution over the sphere.
         *
         * @return  A normalized 3D vector with a length of 1
         */
        static Vec<3> randomUnitVec()
        {
            while (true) {
                const auto p = Vec<3>::random(-1, 1);
                const auto len = std::pow(p.length(), 2);

                if (len <= 1) {
                    return p / std::sqrt(len);
                }
            }
        }

        /**
         * @brief   Computes the Euclidean length (magnitude) of the vector.
         *
         * @returns Length of the vector
         */
        [[nodiscard]] T length() const
        {
            T sum = T{};

            for (size_t k = 0; k < N; k++) {
                sum += std::pow(this->_data[k], 2);
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
        [[nodiscard]] Vec normalized() const
        {
            Vec res = *this;
            return res.normalize();
        }

        /**
         * @brief   Normalizes the vector in-place.
         *
         * @return  Vec&    Reference to the normalized vector
         *
         * @note    If the vector has zero length, it will remain unchanged.
         */
        [[nodiscard]] Vec& normalize()
        {
            T len = this->length();

            if (len == T(0)) {
                return *this;
            }

            for (size_t k = 0; k < N; k++) {
                this->_data[k] /= len;
            }

            return *this;
        }

        /**
         * @brief   Returns a copy of the vector with each component clamped
         *          between min and max.
         *
         * @param   min Minimum bound for clamping
         * @param   max Maximum bound for clamping
         * @returns A new vector with clamped values
         */
        [[nodiscard]] Vec clamped(T min, T max) const
        {
            Vec res = *this;
            return res.clamp(min, max);
        }

        /**
         * @brief   Clamps each component of the vector between min and max
         *          in-place.
         *
         * @param   min Minimum bound for clamping
         * @param   max Maximum bound for clamping
         * @returns Reference to the current (now clamped) vector
         */
        [[nodiscard]] Vec& clamp(T min, T max)
        {
            for (size_t k = 0; k < N; k++) {
                this->_data[k] = std::clamp(this->_data[k], min, max);
            }
            return *this;
        }

        /**
         * @brief   Accesses the element at index i.
         *
         * @param   i   Index of the element
         * @return  Reference to the element
         */
        [[nodiscard]] T& operator[](size_t i) { return this->_data[i]; }

        /**
         * @brief   Accesses the element at index i (const).
         *
         * @param   i   Index of the element
         * @return  Constant reference to the element
         */
        [[nodiscard]] const T& operator[](size_t i) const { return this->_data[i]; }

        V_VEC_OP(+);
        V_VEC_OP(-);
        V_VEC_OP(*);
        V_VEC_OP(/);

        V_SCALAR_OP(+);
        V_SCALAR_OP(-);
        V_SCALAR_OP(*);
        V_SCALAR_OP(/);

        /**
         * @brief   Assignment operator.
         *
         * Copies the contents of another vector to this one.
         *
         * @param   other   The vector to copy from
         * @returns Reference to this vector
         */
        [[nodiscard]] Vec& operator=(const Vec& other)
        {
            if (this != &other) {
                this->_data = other._data;
            }
            return *this;
        }

        /**
         * @brief   Equality operator.
         *
         * Compares each component of two vectors for equality.
         *
         * @param   other   The vector to compare with
         * @return  True if all components are equal, false otherwise
         */
        [[nodiscard]] bool operator==(const Vec& other) const
        {
            for (size_t i = 0; i < N; i++) {
                if (_data[i] != other._data[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
         * @brief   Unary negation operator.
         *
         * Returns a new vector with each component negated.
         *
         * @returns A new vector that is the negation of this one
         */
        Vec operator-() const
        {
            Vec res;

            for (size_t i = 0; i < N; ++i) {
                res[i] = -this->_data[i];
            }
            return res;
        }

        /**
         * @return  Const reference to the data array
         */
        [[nodiscard]] const std::array<T, N>& data() const { return this->_data; }

        /**
         * @return  Reference to the data array
         */
        [[nodiscard]] std::array<T, N>& data() { return this->_data; }

    protected:
        std::array<T, N> _data;
    };

    SCALAR_V_OP(+);
    SCALAR_V_OP(-);
    SCALAR_V_OP(*);
    SCALAR_V_OP(/);

    /**
     * @brief   Computes the dot product between two vectors.
     *
     * @tparam  N   Dimension of the vectors
     * @tparam  T   Type of the elements
     * @param   lhs First vector
     * @param   rhs Second vector
     * @return  The dot product result
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
     * @brief   Computes the reflection of the vector `lhs` across the vector
     *          `rhs`.
     *
     * This operation assumes that `rhs` is a normalized vector.
     * If `rhs` is not normalized, the result will still be a valid reflection,
     * but it may not behave as expected in terms of geometric interpretation.
     *
     * @tparam  N   Dimension of the vectors
     * @tparam  T   Type of the elements
     * @param   lhs The vector to be reflected
     * @param   rhs The vector to reflect across
     * @returns The reflected vector
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

}
