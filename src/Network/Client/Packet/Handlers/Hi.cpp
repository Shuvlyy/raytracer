#include "Hi.hpp"

#include "Logger.hpp"

#include "Network/Packet/Packets/Hi.hpp"

namespace raytracer::network::packet::handler
{

    void
    Hi::handle
    (
        const Packet &
    )
        const
    {

        LOG_DEBUG("[packet::handler::HI] Called");
    }

}
