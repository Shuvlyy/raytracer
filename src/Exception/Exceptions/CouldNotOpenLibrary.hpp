#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class CouldNotOpenLibrary final
        : public AException
    {
    public:
        explicit CouldNotOpenLibrary(
            const std::string &error
        ) : AException("Could not open library: " + error) {}
    };

}
