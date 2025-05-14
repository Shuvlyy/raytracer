#include "Finito.hpp"

#include "logger/Logger.hpp"

#include "Network/Packet/Packets/Finito.hpp"

namespace raytracer::network::packet::server::handler
{

    void
    Finito::handle
    (
        const Packet& packet,
        network::server::Session& session
    )
        const
    {
        LOG_DEBUG("Called (SFD: " + std::to_string(session.getId()) + ")");

        const auto& p = reinterpret_cast<const packet::Finito&>(packet);

        // TODO: Finish this.
    }

}
