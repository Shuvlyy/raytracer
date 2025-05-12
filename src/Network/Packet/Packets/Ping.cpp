#include "Ping.hpp"

#include "Exception/Exceptions/UnexpectedRemainingData.hpp"

#include "../IO/Deserializer.hpp"
#include "../IO/Serializer.hpp"

namespace raytracer::network::packet
{

    Ping::Ping
    (
        const uint64_t timestamp
    )
        : Packet(Type::PING),
          _timestamp(timestamp)
    {}

    ByteBuffer
    Ping::serialize
    ()
        const
    {
        Serializer s;

        s.write<uint8_t>(static_cast<uint8_t>(this->_type));
        s.write<uint64_t>(this->_timestamp);
        return s.data();
    }

    void
    Ping::deserialize
    (
        const uint8_t* data,
        const size_t size
    )
    {
        Deserializer d({ data, data + size });

        const auto rawType = d.read<uint8_t>();
        const Type type = fromRawTypeToType(rawType);

        this->_timestamp = d.read<uint64_t>();

        if (d.hasRemaining()) {
            throw exception::UnexpectedRemainingData(type);
        }
    }

}
