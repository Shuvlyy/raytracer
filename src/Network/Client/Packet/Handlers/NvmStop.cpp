#include "NvmStop.hpp"
#include "logger/Logger.hpp"

namespace raytracer::network::packet::client::handler
{

    void
    NvmStop::handle
    (
        const Packet &,
        app::Client &cli
    )
        const
    {
        LOG_DEBUG("Called.");

        cli.stopRender();
    }

}
