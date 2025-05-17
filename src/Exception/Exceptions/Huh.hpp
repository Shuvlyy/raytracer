#pragma once

#include "Exception/AException.hpp"

#include <string>

namespace raytracer::exception
{

    class Huh final
        : public AException
    {
    public:
        explicit Huh()
            : AException("Huh?")
        {}
    };

}
