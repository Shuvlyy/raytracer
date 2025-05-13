#include "Kiss.hpp"

#include "Exception/Exceptions/UnexpectedRemainingData.hpp"

#include "Network/Packet/IO/Deserializer.hpp"
#include "Network/Packet/IO/Serializer.hpp"

namespace raytracer::network::packet
{

    Kiss::Kiss()
        : Packet(Type::KISS) {}

    ByteBuffer
    Kiss::serialize
    ()
        const
    {
        Serializer s;

        s.write<uint8_t>(static_cast<uint8_t>(this->_type));
        return s.data();
    }

    void
    Kiss::deserialize
    (
        const uint8_t* data,
        const size_t size
    )
    {
        Deserializer d({ data, data + size });

        const auto rawType = d.read<uint8_t>();
        const Type type = fromRawTypeToType(rawType);

        if (d.hasRemaining()) {
            throw exception::UnexpectedRemainingData(type);
        }
    }

}
