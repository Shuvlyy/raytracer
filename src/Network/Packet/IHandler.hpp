#pragma once

#define TRIGGER_RET [[nodiscard]] virtual Type getTrigger() const

#include "Network/Server/Session/Session.hpp"

namespace raytracer::network::packet
{

    class IHandler
    {
    public:
        virtual ~IHandler() = default;

        virtual void handle(
            const Packet &packet,
            server::Session &session
        ) const = 0;

        TRIGGER_RET = 0;
    };

}
