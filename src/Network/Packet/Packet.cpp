#include "Packet.hpp"

#include "Packets/Ping.hpp"
#include "Packets/Pong.hpp"
#include "Packets/Kiss.hpp"
#include "Packets/Workslave.hpp"
#include "Packets/Cestciao.hpp"
#include "Packets/Finito.hpp"
#include "Packets/Nvmstop.hpp"

#include "Exception/Exceptions/EmptyByteBuffer.hpp"
#include "Exception/Exceptions/UnknownPacket.hpp"

namespace raytracer::network
{

    Packet::Packet
    (
        const packet::Type type
    )
        : _type(type)
    {}

    std::unique_ptr<Packet>
    Packet::fromByteBuffer
    (
        const ByteBuffer& buf
    )
    {
        if (buf.empty()) {
            throw exception::EmptyByteBuffer();
        }

        const uint8_t rawPacketType = buf[0];

        const auto packetType = fromRawTypeToType(rawPacketType);

        std::unique_ptr<Packet> packet;

        /* TODO: Move this switch case in a dedicated function. */
        switch (packetType) {
            case packet::Type::PING:
                packet = std::make_unique<packet::Ping>();
                break;
            case packet::Type::PONG:
                packet = std::make_unique<packet::Pong>();
                break;
            case packet::Type::KISS:
                packet = std::make_unique<packet::Kiss>();
                break;
            case packet::Type::WORKSLAVE:
                packet = std::make_unique<packet::Workslave>();
                break;
            case packet::Type::CESTCIAO:
                packet = std::make_unique<packet::Cestciao>();
                break;
            case packet::Type::FINITO:
                packet = std::make_unique<packet::Finito>();
                break;
            case packet::Type::NVMSTOP:
                packet = std::make_unique<packet::Nvmstop>();
                break;
            default:
                throw exception::UnknownPacket();
        }
        /* --------------------------------------------------- */

        packet->deserialize(buf.data(), buf.size());
        return packet;
    }

}
