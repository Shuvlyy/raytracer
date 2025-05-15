#include "Finito.hpp"
#include "Network/Packet/Packets/Finito.hpp"
#include "Network/Server/Server.hpp"
#include "logger/Logger.hpp"

namespace raytracer::network::packet::server::handler
{

    void
    Finito::handle
    (
        const Packet& packet,
        Server& server,
        network::server::Session& session
    )
        const
    {
        const auto& p = reinterpret_cast<const packet::Finito&>(packet);

        session.setState(network::server::session::State::READY);
        session.getData().result = p.getPixelBuffer();

        LOG_DEBUG("Client (SFD: " + std::to_string(session.getId()) + ") finished rendering.");
    }

}
