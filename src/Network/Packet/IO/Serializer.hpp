#pragma once

#include "../Packet.hpp"

#include "Common/Utils.hpp"

#include "Math/Vec.hpp"

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

        template<typename T>
        void writeVector(const std::vector<T>& vec)
        {
            this->write<uint32_t>(vec.size());
            for (const auto& item : vec) {
                this->write(item);
            }
        }

        template<size_t N, typename T>
        void write(const math::Vec<N, T>& vec)
        {
            for (const auto& val : vec.data()) {
                this->write(val);
            }
        }

        void write(bool value);
        void write(const std::string& str);

        void clear();

        [[nodiscard]] const ByteBuffer& data() const { return this->_buf; }

    private:
        ByteBuffer _buf;
    };

}
