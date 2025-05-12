#pragma once

#include "../Packet.hpp"

namespace raytracer::network::packet
{

    class Ping final
        : public Packet
    {
    public:
        explicit Ping(uint64_t timestamp = 0);

        [[nodiscard]] ByteBuffer serialize() const override;
        void deserialize(const uint8_t* data, size_t size) override;

        [[nodiscard]] uint64_t getTimestamp() const { return this->_timestamp; }

    private:
        uint64_t _timestamp;
    };

}
