#pragma once

#include "Network/Packet/Handler/IHandler.hpp"

namespace raytracer::network::packet::handler
{

    class Hi final
        : public IHandler
    {
    public:
        void handle(
            const Packet &packet
        ) const override;

        [[nodiscard]] Type getTrigger()
            const override
        {
            return Type::HI;
        }
    };

}
