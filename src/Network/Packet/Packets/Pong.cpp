#include "Pong.hpp"

#include "Exception/Exceptions/UnexpectedRemainingData.hpp"

#include "Network/Packet/IO/Deserializer.hpp"
#include "Network/Packet/IO/Serializer.hpp"

namespace raytracer::network::packet
{

    Pong::Pong
    (
        const uint64_t timestamp,
        const uint8_t progress
    ) : Packet(Type::PONG),
        _timestamp(timestamp),
        _progress(progress)
    {}

    ByteBuffer
    Pong::serialize
    ()
        const
    {
        Serializer s;

        s.write<uint8_t>(static_cast<uint8_t>(this->_type));
        s.write<uint64_t>(this->_timestamp);
        s.write<uint8_t>(this->_progress);
        return s.data();
    }

    void
    Pong::deserialize
    (
        const uint8_t* data,
        const size_t size
    )
    {
        Deserializer d({ data, data + size });

        const auto rawType = d.read<uint8_t>();
        const Type type = fromRawTypeToType(rawType);

        this->_timestamp = d.read<uint64_t>();
        this->_progress = d.read<uint8_t>();

        if (d.hasRemaining()) {
            throw exception::UnexpectedRemainingData(type);
        }
    }

}
