#include "WorkSlave.hpp"

#include "logger/Logger.hpp"

namespace raytracer::network::packet::client::handler
{

    void
    WorkSlave::handle
    (
        const Packet &packet,
        Client &cli
    )
        const
    {
        LOG_DEBUG("[packet::handler::WORKSLAVE] Called.");

    }

}
