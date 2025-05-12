#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class UnknownPacket final
        : public AException
    {
    public:
        explicit UnknownPacket()
            : AException("Tried to handle an unknown/unregistered packet.")
        {}
    };

}
