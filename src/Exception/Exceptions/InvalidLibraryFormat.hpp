#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class InvalidLibraryFormat final
        : public AException
    {
    public:
        explicit InvalidLibraryFormat()
            : AException("Wrong library format.") {}
    };

}
