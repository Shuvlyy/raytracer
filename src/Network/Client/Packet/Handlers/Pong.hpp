#pragma once

#include "Network/Packet/IHandler.hpp"

namespace raytracer::network::packet::handler
{

    class Pong final
        : public IHandler
    {
    public:
        void handle(
            const Packet &packet
        ) const override;

        [[nodiscard]] Type getTrigger()
            const override
        {
            return Type::PONG;
        }
    };

}
