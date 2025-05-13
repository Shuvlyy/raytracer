#pragma once

#include "Network/Packet/Packet.hpp"

namespace raytracer::network::packet
{

    class Nvmstop final
        : public Packet
    {
    public:
        explicit Nvmstop();

        [[nodiscard]] ByteBuffer serialize() const override;
        void deserialize(const uint8_t* data, size_t size) override;
    };

}
