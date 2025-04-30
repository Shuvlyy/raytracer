#pragma once

#include "Vec.hpp"

namespace raytracer::math
{

    /**
     * @brief   Represents a square matrix of size NxN.
     *
     * @tparam  N   The dimension of the matrix (number of rows and columns)
     * @tparam  T   The numeric type of the matrix elements
     */
    template<size_t N, typename T = double>
    requires std::is_arithmetic_v<T>
    class Mat final
    {
    public:

        /**
         * @brief   Constructs a matrix from an initializer list.
         *
         * @param   init    A nested initializer list specifying the matrix
         *                  values
         *
         * @throws  std::invalid_argument   if the initializer list does not
         *                                  have exactly N rows and N columns.
         */
        Mat(std::initializer_list<std::initializer_list<T>> init)
        {
            if (init.size() != N) {
                throw std::invalid_argument("Matrix must have exactly N rows");
            }

            std::size_t i = 0;
            for (const auto& row : init) {
                if (row.size() != N) {
                    throw std::invalid_argument("Each matrix row must have exactly N columns");
                }

                std::size_t j = 0;
                for (const auto& value : row) {
                    this->_data[i][j] = value;
                    ++j;
                }
                ++i;
            }
        }

        /**
         * @brief   Default constructor. Initializes all elements to zero.
         */
        Mat()
        {
            for (auto& row : this->_data) {
                row.fill(T{});
            }
        }

        /**
         * @brief   Creates a 3x3 rotation matrix from Euler angles
         *          (yaw, pitch, roll).
         *
         * @param   yaw     Y-axis rotation (in radians)
         * @param   pitch   X-axis rotation (in radians)
         * @param   roll    Z-axis rotation (in radians)
         *
         * @returns A 3x3 rotation matrix representing the combined rotations.
         *
         * @note    The rotation order is roll * pitch * yaw.
         */
        static Mat<3> fromEuler
        (
            const double yaw,
            const double pitch,
            const double roll
        )
        {
            const double cy = std::cos(yaw);
            const double sy = std::sin(yaw);
            const double cp = std::cos(pitch);
            const double sp = std::sin(pitch);
            const double cr = std::cos(roll);
            const double sr = std::sin(roll);

            return Mat<3>({
                { cr * cy + sr * sp * sy, -sr * cp, cr * sy - sr * sp * cy },
                { sr * cy - cr * sp * sy, cr * cp, sr * sy + cr * sp * cy },
                { -cp * sy, sp, cp * cy }
            });
        }

        /**
         * @brief Multiplies the matrix by a vector.
         *
         * @tparam  U   The numeric type of the vector elements (defaults to T)
         * @param   v   The vector to multiply
         * @returns A new vector resulting from the multiplication
         */
        template<typename U = T>
        Vec<N, U> operator*(const Vec<N, U>& v) const
        {
            Vec<N, U> res{};

            for (std::size_t i = 0; i < N; ++i) {
                for (std::size_t j = 0; j < N; ++j) {
                    res[i] += static_cast<U>(this->_data[i][j]) * v[j];
                }
            }
            return res;
        }

        /**
         * @brief   Multiplies the matrix by another matrix.
         *
         * @param   other   The matrix to multiply with
         * @returns A new matrix resulting from the multiplication
         */
        Mat operator*(const Mat& other) const
        {
            Mat res{};

            for (std::size_t i = 0; i < N; ++i) {
                for (std::size_t j = 0; j < N; ++j) {
                    for (std::size_t k = 0; k < N; ++k) {
                        res._data[i][j] +=
                            this->_data[i][k] * other._data[k][j];
                    }
                }
            }
            return res;
        }

        /**
         * @brief   Accesses a row of the matrix.
         *
         * @param   row Row index
         * @returns A reference to the specified row
         */
        std::array<T, N>& operator[](std::size_t row) { return _data[row]; }

        /**
         * @brief   Accesses a row of the matrix.
         *
         * @param   row Row index
         * @returns A const reference to the specified row.
         */
        const std::array<T, N>& operator[](std::size_t row) const { return _data[row]; }

    private:
        std::array<std::array<T, N>, N> _data;
    };

}
