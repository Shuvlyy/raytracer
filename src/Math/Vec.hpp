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
    Vec operator op(T scalar) const {                                         \
        Vec res = *this;                                                      \
        std::for_each(                                                        \
            res._data.begin(),                                                \
            res._data.end(),                                                  \
            [=](T& x) {                                                       \
                x = x op scalar;                                              \
            });                                                               \
        return res;                                                           \
    }                                                                         \
                                                                              \
    Vec& operator op##=(T scalar) {                                           \
        std::for_each(                                                        \
            this->_data.begin(),                                              \
            this->_data.end(),                                                \
            [=](T& x) {                                                       \
                x = x op scalar;                                              \
            });                                                               \
        return *this;                                                         \
    }

#include <array>
#include <algorithm>
#include <cmath>

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

        /**
         * @return  const std::array<T, N>& Const reference to the data array
         */
        const std::array<T, N>& data() const { return _data; }

        /**
         * @return  std::array<T, N>&   Reference to the data array
         */
        std::array<T, N>& data() { return _data; }

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

}
