#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class InvalidUsage final
        : public AException
    {
    public:
        explicit InvalidUsage()
            : AException("Invalid usage. Use --help flag for help.")
        {}
    };

}
