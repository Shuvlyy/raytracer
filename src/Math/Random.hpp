#pragma once

#include <random>

namespace raytracer::math
{

    static inline double randomDouble(
        const double min = 0.0,
        const double max = 1.0
    )
    {
        static std::uniform_real_distribution<double> distribution(min, max);
        static std::mt19937 generator;
        return distribution(generator);
    }

}
