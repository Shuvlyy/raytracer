#pragma once

#define TRIGGER_RET [[nodiscard]] virtual Type getTrigger() const

#include "Network/Packet/Packet.hpp"
#include "App/Client/Client.hpp"

namespace raytracer::network::packet::client
{

    class IHandler
    {
    public:
        virtual ~IHandler() = default;

        virtual void handle(
            const Packet& packet,
             app::Client &cli
        ) const = 0;

        TRIGGER_RET = 0;
    };

}
