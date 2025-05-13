#pragma once

#define TRIGGER_RET [[nodiscard]] virtual Type getTrigger() const

#include "Network/Client/Client.hpp"

namespace raytracer::network::packet::client
{

    class IHandler
    {
    public:
        virtual ~IHandler() = default;

        virtual void handle(
            const Packet& packet,
            Client &cli
        ) const = 0;

        TRIGGER_RET = 0;
    };

}
