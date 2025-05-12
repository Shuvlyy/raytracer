#include "Serializer.hpp"

#include "Exception/Exceptions/ValueOverflow.hpp"

#include "Logger.hpp"

#include <limits>

namespace raytracer::network::packet
{

    void
    Serializer::write
    (
        const void* data,
        const size_t size
    )
    {
        const auto bytes = static_cast<const uint8_t *>(data);

        this->_buf.insert(
            this->_buf.end(),
            bytes,
            bytes + size
        );
    }

    void
    Serializer::write
    (
        const bool value
    )
    {
        const uint8_t byte = value ? 1 : 0;

        this->write(byte);
    }

    void
    Serializer::write
    (
        const std::string& str
    )
    {
        if (str.size() > std::numeric_limits<uint32_t>::max()) {
            throw exception::ValueOverflow(str.size());
        }

        LOG_DEBUG("String size: " + std::to_string(str.size()));
        this->write<uint32_t>(str.size());
        this->write(str.data(), str.size());
    }

    void
    Serializer::clear
    ()
    {
        this->_buf.clear();
    }

}
