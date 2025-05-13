#pragma once

#include "Network/Packet/Packet.hpp"

namespace raytracer::network::packet
{

    class Pong final
        : public Packet
    {
    public:
        explicit Pong(uint64_t timestamp = 0, uint8_t progress = 0);

        [[nodiscard]] ByteBuffer serialize() const override;
        void deserialize(const uint8_t* data, size_t size) override;

        [[nodiscard]] uint64_t getTimestamp() const { return this->_timestamp; }
        [[nodiscard]] uint8_t getProgress() const { return this->_progress; }

    private:
        uint64_t _timestamp;
        uint8_t _progress;
    };

}
