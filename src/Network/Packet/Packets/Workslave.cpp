#include "Workslave.hpp"

#include "Exception/Exceptions/UnexpectedRemainingData.hpp"

#include "Network/Packet/IO/Deserializer.hpp"
#include "Network/Packet/IO/Serializer.hpp"

namespace raytracer::network::packet
{

    Workslave::Workslave
    (
        const std::string& sceneContent,
        const uint32_t fromY,
        const uint32_t toY
    ) : Packet(Type::WORKSLAVE),
        _sceneContent(sceneContent),
        _fromY(fromY), _toY(toY)
    {}

    ByteBuffer
    Workslave::serialize
    ()
        const
    {
        Serializer s;

        s.write<uint8_t>(static_cast<uint8_t>(this->_type));
        s.write(this->_sceneContent);
        s.write<uint32_t>(this->_fromY);
        s.write<uint32_t>(this->_toY);
        return s.data();
    }

    void
    Workslave::deserialize
    (
        const uint8_t* data,
        const size_t size
    )
    {
        Deserializer d({ data, data + size });

        const auto rawType = d.read<uint8_t>();
        const Type type = fromRawTypeToType(rawType);

        this->_sceneContent = d.readString();
        this->_fromY = d.read<uint32_t>();
        this->_toY = d.read<uint32_t>();

        if (d.hasRemaining()) {
            throw exception::UnexpectedRemainingData(type);
        }
    }

}
