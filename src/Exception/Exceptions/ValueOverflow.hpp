#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class ValueOverflow final
        : public AException
    {
    public:
        explicit ValueOverflow(const int value)
            : AException("Value overflow: " + std::to_string(value))
        {}
    };

}
