#pragma once

#include "Network/Packet/Packet.hpp"

namespace raytracer::network::packet
{

    class Cestciao final
        : public Packet
    {
    public:
        explicit Cestciao();

        [[nodiscard]] ByteBuffer serialize() const override;
        void deserialize(const uint8_t* data, size_t size) override;
    };

}
