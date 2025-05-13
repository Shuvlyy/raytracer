#include "NvmStop.hpp"
#include "logger/Logger.hpp"

namespace raytracer::network::packet::client::handler
{

    void
    NvmStop::handle
    (
        const Packet &packet,
        Client &cli
    )
        const
    {
        LOG_DEBUG("[packet::handler::NVMSTOP] Called.");

    }

}
