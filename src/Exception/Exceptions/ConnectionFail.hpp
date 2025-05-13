#pragma once

#include "Exception/AException.hpp"

#include <string>

namespace raytracer::exception
{
    class ConnectionFail final
        : public AException
    {
    public:
        explicit ConnectionFail(const int port, const std::string &address, const std::string &errorMessage)
            : AException("Couldn't Connect to " + address + " on port " + std::to_string(port) + " : " + errorMessage + ".\n")
        {}
    };

}