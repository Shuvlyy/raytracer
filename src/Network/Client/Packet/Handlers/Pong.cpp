#include "Pong.hpp"

#include "Network/Packet/Packets/Pong.hpp"
#include "Logger.hpp"

namespace raytracer::network::packet::handler
{

    void
    Pong::handle
    (
        const Packet &
    )
        const
    {
        LOG_DEBUG("[packet::handler::PONG] Called");
    }

}
