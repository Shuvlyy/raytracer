#pragma once

#include "Network/Packet/Packet.hpp"

namespace raytracer::network::packet
{

    class Kiss final
        : public Packet
    {
    public:
        explicit Kiss();

        [[nodiscard]] ByteBuffer serialize() const override;
        void deserialize(const uint8_t* data, size_t size) override;
    };

}
