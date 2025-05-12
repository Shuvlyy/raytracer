#pragma once

#include <cstring>

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class StandardFunctionFail final
        : public AException
    {
    public:
        explicit StandardFunctionFail(const std::string &functionName)
            : AException(
                "Standard function fail: " + functionName + ": " + strerror(errno)
            )
        {}
    };

}
