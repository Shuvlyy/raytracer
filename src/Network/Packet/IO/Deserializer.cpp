#include "Deserializer.hpp"

#include "Exception/Exceptions/ValueOverflow.hpp"
#include "Exception/Exceptions/InvalidPacketSize.hpp"

namespace raytracer::server::packet
{

    Deserializer::Deserializer
    (
        ByteBuffer data
    )
        : _buf(std::move(data)),
          _offset(0)
    {}

    bool
    Deserializer::readBool
    ()
    {
        const auto byte = this->read<uint8_t>();

        if (byte > 1) {
            throw exception::ValueOverflow(byte);
        }
        return byte == 1;
    }

    std::string
    Deserializer::readString
    ()
    {
        const auto length = this->read<uint32_t>();

        this->ensureAvailable(length);

        std::string res(
            reinterpret_cast<const char *>(this->_buf.data() + this->_offset),
            length
        );

        this->_offset += length;
        return res;
    }

    bool
    Deserializer::hasRemaining
    ()
        const
    {
        return this->_offset < this->_buf.size();
    }

    size_t
    Deserializer::remaining
    ()
        const
    {
        return this->_buf.size() - this->_offset;
    }

    void
    Deserializer::ensureAvailable
    (
        const size_t size
    )
        const
    {
        if (this->_offset + size > this->_buf.size()) {
            throw exception::InvalidPacketSize(this->_offset + size, this->_buf.size());
        }
    }

}
