#include "Ping.hpp"

#include "Logger.hpp"

#include "Network/Packet/Packets/Ping.hpp"
#include "Network/Packet/Packets/Pong.hpp"

namespace raytracer::network::packet::server::handler
{

    void
    Ping::handle
    (
        const Packet &packet,
        network::server::Session &session
    )
        const
    {
        LOG_DEBUG("Called (SFD: " + std::to_string(session.getId()) + ")");

        const auto &p = reinterpret_cast<const packet::Ping &>(packet);

        const Pong pongPacket(p.getTimestamp());

        session.getControlSocket().sendPacket(pongPacket.serialize());
    }

}
