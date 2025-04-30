#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class CouldNotReadLibraryFunction final
        : public AException
    {
    public:
        explicit CouldNotReadLibraryFunction(
            const std::string &functionName,
            const std::string &errorMessage
        ) : AException(
                functionName + ": Could not read library function.\n"
                + errorMessage
            ) {}
    };

}
