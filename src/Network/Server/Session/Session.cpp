#include "Session.hpp"
#include "Common/Utils.hpp"
#include "Network/Packet/Packets/Ping.hpp"

namespace raytracer::network::server
{

    Session::Session
    (
        const int fd,
        const uint32_t id
    ) : _id(id),
        _controlSocket(Socket(fd)),
        _latency(0),
        _lastLatencyRefresh(std::chrono::steady_clock::now()),
        _state(session::State::READY)
    {}

    void
    Session::refreshLatency
    ()
    {
        const packet::Ping p(utils::getCurrentTimestamp());

        this->getControlSocket().sendPacket(p.serialize());
    }

}
