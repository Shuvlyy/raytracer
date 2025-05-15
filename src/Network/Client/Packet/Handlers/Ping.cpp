#include "Ping.hpp"
#include "logger/Logger.hpp"

#include "Network/Packet/Packets/Ping.hpp"
#include "Network/Packet/Packets/Pong.hpp"

namespace raytracer::network::packet::client::handler
{

    void
    Ping::handle
    (
        const Packet &packet,
        app::Client &cli
    )
        const
    {
        LOG_DEBUG("Called.");

        const auto &p = reinterpret_cast<const packet::Ping &>(packet);

        uint8_t progress = renderer::totalComputedPixels.load() / cli.getTotal() * 100;
        cli.push(std::make_unique<Pong>(p.getTimestamp(), progress));
    }

}
