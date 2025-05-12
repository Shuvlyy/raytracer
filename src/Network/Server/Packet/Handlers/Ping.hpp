#pragma once

#include "../../../Packet/IHandler.hpp"

namespace raytracer::server::packet::handler
{

    class Ping final
        : public IHandler
    {
    public:
        void handle(
            const Packet &packet,
            Session &session
        ) const override;

        TRIGGER_RET override { return Type::PING; }
    };

}
