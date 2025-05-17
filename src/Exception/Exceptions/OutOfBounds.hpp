#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class OutOfBounds final
        : public AException
    {
    public:
        explicit OutOfBounds(const int value)
            : AException("Out of bounds: " + std::to_string(value))
        {}
    };

}
