#pragma once

#include "Network/Client/Packet/IHandler.hpp"

namespace raytracer::network::packet::client::handler
{

    class NvmStop final
        : public IHandler
    {
    public:
        void handle(
            const Packet &packet,
            Client &cli
        ) const override;

        [[nodiscard]] Type getTrigger()
            const override
        {
            return Type::NVMSTOP;
        }
    };

}
