#include "Cestciao.hpp"

#include "logger/Logger.hpp"

#include "Network/Packet/Packets/Cestciao.hpp"

namespace raytracer::network::packet::server::handler
{

    void
    Cestciao::handle
    (
        const Packet&,
        Server&,
        network::server::Session& session
    )
        const
    {
        LOG_DEBUG("Called (SFD: " + std::to_string(session.getId()) + ")");

        session.setState(network::server::session::State::DEADASS);

        // TODO: Finish this.
    }

}
