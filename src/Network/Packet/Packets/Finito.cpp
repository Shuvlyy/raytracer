#include "Finito.hpp"

#include "Exception/Exceptions/UnexpectedRemainingData.hpp"

#include "Network/Packet/IO/Deserializer.hpp"
#include "Network/Packet/IO/Serializer.hpp"

namespace raytracer::network::packet
{

    Finito::Finito(const PixelBuffer& pixelBuffer)
        : Packet(Type::PONG), _pixelBuffer(pixelBuffer) {}

    ByteBuffer
    Finito::serialize
    ()
        const
    {
        Serializer s;

        s.write<uint8_t>(static_cast<uint8_t>(this->_type));

        s.write<uint32_t>(this->_pixelBuffer.size());
        for (const auto& pixel : this->_pixelBuffer) {
            for (const auto& color : pixel.data()) {
                s.write<double>(color);
            }
        }
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

        const auto s = d.read<uint32_t>();

        this->_pixelBuffer.resize(s);

        for (uint32_t k = 0; k < s; ++k) {
            for (uint32_t i = 0; i < 3; ++i) {
                this->_pixelBuffer[k][i] = d.read<double>();
            }
        }

        if (d.hasRemaining()) {
            throw exception::UnexpectedRemainingData(type);
        }
    }

}
