#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class SocketFail final
        : public AException
    {
    public:
        explicit SocketFail(const std::string &errorMessage)
            : AException("Couldn't create socket: " + errorMessage + ".\n")
        {}
    };

}