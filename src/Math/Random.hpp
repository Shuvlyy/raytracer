#pragma once

#include <random>

namespace raytracer::math
{

    /**
     * @brief Generates a random double within a specified range.
     *
     * @param   min The minimum bound for the random number (inclusive).
     * @param   max The maximum bound for the random number (exclusive).
     * @returns A random double value between min (inclusive) and max
     *          (exclusive).
     */
    static double randomDouble(
        const double min = 0.0,
        const double max = 1.0
    )
    {
        static std::uniform_real_distribution distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }

}
