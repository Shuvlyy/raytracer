#pragma once

#include "../Packet.hpp"

#include "Common/Utils.hpp"
#include "Math/Vec.hpp"

#include <cstring>

#include <netinet/in.h>

namespace raytracer::network::packet
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

        template<typename T>
        std::vector<T> readVector()
        {
            const auto size = this->read<uint32_t>();

            std::vector<T> vec;
            vec.reserve(size);
            for (uint32_t i = 0; i < size; ++i) {
                vec.push_back(this->read<T>());
            }
            return vec;
        }

        template<size_t N, typename T>
        math::Vec<N, T> readVec()
        {
            std::array<T, N> data;
            for (size_t i = 0; i < N; ++i) {
                data[i] = this->read<T>();
            }
            math::Vec<N, T> vec;
            vec.data() = data;
            return vec;
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

    template<>
    inline math::Vec<3> Deserializer::read<math::Vec<3>>()
    {
        return this->readVec<3, double>();
    }

}
