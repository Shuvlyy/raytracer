#pragma once

#include "../Packet.hpp"

#include "Common/Utils.hpp"

#include <string>
#include <arpa/inet.h>

namespace raytracer::network::packet
{

    class Serializer final
    {
    public:
        void write(
            const void* data,
            size_t size
        );

        template<typename T>
        void write(const T& value)
        {
            static_assert(
                std::is_trivially_copyable_v<T>,
                "Serializer::write(T) only supports trivially copyable types."
            );

            T toWrite = value;

            /* Endianness */
            switch (sizeof(T)) {
                case 2:
                    toWrite = htons(toWrite);
                    break;
                case 4:
                    toWrite = htonl(toWrite);
                    break;
                case 8:
                    toWrite = utils::htonll(toWrite);
                    break;
                default:
                    break;
            }

            this->write(&toWrite, sizeof(T));
        }

        void write(bool value);
        void write(const std::string& str);

        void clear();

        [[nodiscard]] const ByteBuffer& data() const { return this->_buf; }

    private:
        ByteBuffer _buf;
    };

}
