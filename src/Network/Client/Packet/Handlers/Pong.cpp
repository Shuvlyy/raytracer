#include "Pong.hpp"

#include "Network/Packet/Packets/Pong.hpp"
#include "logger/Logger.hpp"

namespace raytracer::network::packet::handler
{

    void
    Pong::handle
    (
        const Packet &,
        Client &cli
    )
        const
    {
        LOG_DEBUG("[packet::handler::PONG] Called");
    }

}
