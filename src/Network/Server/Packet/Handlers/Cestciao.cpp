#include "Cestciao.hpp"

#include "logger/Logger.hpp"

#include "Network/Packet/Packets/Cestciao.hpp"

namespace raytracer::network::packet::server::handler
{

    void
    Cestciao::handle
    (
        const Packet& packet,
        Server& server,
        network::server::Session& session
    )
        const
    {
        LOG_DEBUG("Called (SFD: " + std::to_string(session.getId()) + ")");

        const auto& p = reinterpret_cast<const packet::Cestciao&>(packet);

        session.setState(network::server::session::State::DEADASS);

        // TODO: Finish this.
    }

}
