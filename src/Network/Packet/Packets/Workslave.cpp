#include "Workslave.hpp"

#include "Exception/Exceptions/UnexpectedRemainingData.hpp"

#include "Network/Packet/IO/Deserializer.hpp"
#include "Network/Packet/IO/Serializer.hpp"

namespace raytracer::network::packet
{

    Workslave::Workslave
    (
        const std::string& sceneContent,
        const uint32_t x,
        const uint32_t y,
        const uint32_t width,
        const uint32_t height
    ) : Packet(Type::WORKSLAVE),
        _sceneContent(sceneContent),
        _x(x), _y(y), _width(width), _height(height)
    {}

    ByteBuffer
    Workslave::serialize
    ()
        const
    {
        Serializer s;

        s.write<uint8_t>(static_cast<uint8_t>(this->_type));
        s.write(this->_sceneContent);
        s.write<uint32_t>(this->_x);
        s.write<uint32_t>(this->_y);
        s.write<uint32_t>(this->_width);
        s.write<uint32_t>(this->_height);
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
        this->_x = d.read<uint32_t>();
        this->_y = d.read<uint32_t>();
        this->_width = d.read<uint32_t>();
        this->_height = d.read<uint32_t>();

        if (d.hasRemaining()) {
            throw exception::UnexpectedRemainingData(type);
        }
    }

}
