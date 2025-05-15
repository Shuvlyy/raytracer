#include "Kiss.hpp"
#include "Network/Packet/Packets/Kiss.hpp"
#include "logger/Logger.hpp"

namespace raytracer::network::packet::client::handler
{

    void
    Kiss::handle
    (
        const Packet &,
        app::Client &cli
    )
        const
    {
        LOG_DEBUG("[packet::handler::KISS] Called.");

        cli.stop();
    }

}
