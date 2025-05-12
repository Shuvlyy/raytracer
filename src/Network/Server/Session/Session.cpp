#include "Session.hpp"

#include "Common/Utils.hpp"

#include "../../Packet/Packets/Ping.hpp"

namespace raytracer::network::server
{

    Session::Session
    (
        const int fd,
        const uint32_t id
    )
        : _id(id),
          _controlSocket(Socket(fd)),
          _latency(0)
    {}

    void
    Session::refreshLatency
    ()
    {
        const packet::Ping p(utils::getCurrentTimestamp());

        this->getControlSocket().sendPacket(p.serialize());
    }

}
