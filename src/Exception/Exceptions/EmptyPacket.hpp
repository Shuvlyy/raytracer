#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class EmptyPacket final
        : public AException
    {
    public:
        explicit EmptyPacket()
            : AException("Empty Packet received.\n")
        {}
    };

}
