#include "Packet.hpp"

#include "Packets/Ping.hpp"
#include "Packets/Pong.hpp"

#include "Exception/Exceptions/EmptyByteBuffer.hpp"
#include "Exception/Exceptions/UnknownPacket.hpp"

namespace raytracer::server
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
        const ByteBuffer &buf
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
            default:
                throw exception::UnknownPacket();
        }
        /* --------------------------------------------------- */

        packet->deserialize(buf.data(), buf.size());
        return packet;
    }

}
