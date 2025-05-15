#pragma once

#include "Image/Image.hpp"
#include "Network/Packet/Packet.hpp"

namespace raytracer::network::packet
{

    class Finito final
        : public Packet
    {
    public:
        explicit Finito(const PixelBuffer& pixelBuffer = {});

        [[nodiscard]] ByteBuffer serialize() const override;
        void deserialize(const uint8_t* data, size_t size) override;

        [[nodiscard]] const PixelBuffer& getPixelBuffer() const
            { return this->_pixelBuffer; }

        void setPixelBuffer(const PixelBuffer &pixelBuffer)
            { this->_pixelBuffer = pixelBuffer; }

    private:
        PixelBuffer _pixelBuffer;
    };

}
