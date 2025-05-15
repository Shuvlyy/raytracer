#include "Pong.hpp"

#include "Common/Utils.hpp"
#include "logger/Logger.hpp"

#include "Network/Packet/Packets/Pong.hpp"

namespace raytracer::network::packet::server::handler
{

    void
    Pong::handle
    (
        const Packet& packet,
        Server& server,
        network::server::Session& session
    )
        const
    {
        LOG_DEBUG("Called (SFD: " + std::to_string(session.getId()) + ")");

        const auto& p = reinterpret_cast<const packet::Pong&>(packet);

        const uint64_t t1 = utils::getCurrentTimestamp();
        const uint64_t t = p.getTimestamp();

        session.setLatency(t1 - t);
    }

}
