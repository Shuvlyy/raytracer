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
        Client &cli
    )
        const
    {
        LOG_DEBUG("[packet::handler::PING] Called.");

        const auto &p = reinterpret_cast<const packet::Ping &>(packet);

        const Pong pongPacket(p.getTimestamp());
    }

}
