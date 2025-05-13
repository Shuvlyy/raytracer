#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class ServerDisconnected final
        : public AException
    {
    public:
        explicit ServerDisconnected()
            : AException(
                "Server disconnected.\n"
            )
        {}
    };

}
