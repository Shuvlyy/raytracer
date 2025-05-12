#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class UnexpectedRemainingData final
        : public AException
    {
    public:
        explicit UnexpectedRemainingData(const network::packet::Type packetType)
            : AException(
                "Unexpected Packet remaining data on Packet : " +
                network::Packet::fromTypeToString(packetType)
            )
        {}
    };

}
