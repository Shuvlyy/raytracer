#pragma once

#include "Network/Server/Packet/IHandler.hpp"

namespace raytracer::network::packet::server::handler
{

    class Finito final
        : public IHandler
    {
    public:
        void handle(
            const Packet& packet,
            network::server::Session& session
        ) const override;

        TRIGGER_RET override { return Type::FINITO; }
    };

}
