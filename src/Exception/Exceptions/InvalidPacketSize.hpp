#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class InvalidPacketSize final
        : public AException
    {
    public:
        explicit InvalidPacketSize(
            const size_t size,
            const size_t maxSize
        )
            : AException(
                "Invalid packet size, got " +
                std::to_string(size) +
                " but max is " +
                std::to_string(maxSize) +
                "."
            )
        {}
    };

}
