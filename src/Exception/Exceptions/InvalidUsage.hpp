#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class InvalidUsage final
        : public AException
    {
    public:
        explicit InvalidUsage(const std::string& reason)
            : AException(
                "Invalid usage: " + reason + ".\nUse --help flag for help."
            )
        {}
    };

}
