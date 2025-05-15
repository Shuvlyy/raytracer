#include "Finito.hpp"

#include "Exception/Exceptions/UnexpectedRemainingData.hpp"

#include "Network/Packet/IO/Deserializer.hpp"
#include "Network/Packet/IO/Serializer.hpp"

namespace raytracer::network::packet
{

    Finito::Finito(const PixelBuffer& pixelBuffer)
        : Packet(Type::FINITO), _pixelBuffer(pixelBuffer) {}

    ByteBuffer
    Finito::serialize
    ()
        const
    {
        Serializer s;

        s.write<uint8_t>(static_cast<uint8_t>(this->_type));

        s.writeVector(this->_pixelBuffer);

        return s.data();
    }

    void
    Finito::deserialize
    (
        const uint8_t* data,
        const size_t size
    )
    {
        Deserializer d({ data, data + size });

        const auto rawType = d.read<uint8_t>();
        const Type type = fromRawTypeToType(rawType);

        this->_pixelBuffer = d.readVector<math::Color>();

        if (d.hasRemaining()) {
            throw exception::UnexpectedRemainingData(type);
        }
    }

}