#pragma once

#include "../Packet.hpp"

#include "Common/Utils.hpp"

#include <cstring>

#include <netinet/in.h>

namespace raytracer::server::packet
{

    class Deserializer final
    {
    public:
        explicit Deserializer(ByteBuffer data);

        template<typename T>
        T read()
        {
            static_assert(
                std::is_trivially_copyable_v<T>,
                "Deserializer::read<T> requires trivially copyable types"
            );

            this->ensureAvailable(sizeof(T));

            T value;

            memcpy(&value, this->_buf.data() + this->_offset, sizeof(T));

            /* Endianness */
            switch (sizeof(T)) {
                case 2:
                    value = ntohs(value);
                    break;
                case 4:
                    value = ntohl(value);
                    break;
                case 8:
                    value = utils::ntohll(value);
                    break;
                default:
                    break;
            }

            this->_offset += sizeof(T);
            return value;
        }

        bool readBool();
        std::string readString();

        [[nodiscard]] bool hasRemaining() const;
        [[nodiscard]] size_t remaining() const;

    private:
        const ByteBuffer _buf;
        size_t _offset;

        void ensureAvailable(size_t size) const;
    };

}
