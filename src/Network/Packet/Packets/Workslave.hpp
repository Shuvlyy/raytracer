#pragma once

#include "Network/Packet/Packet.hpp"

#include <string>
#include <cstdint>

namespace raytracer::network::packet
{

    class Workslave final
        : public Packet
    {
    public:
        explicit Workslave(
            const std::string& sceneContent = "",
            uint32_t fromY = 0,
            uint32_t toY = 0
        );

        [[nodiscard]] ByteBuffer serialize() const override;
        void deserialize(const uint8_t* data, size_t size) override;

        void setSceneContent(const std::string& sceneContent)
            { this->_sceneContent = sceneContent; }
        void setFromY(const uint32_t fromY) { this->_fromY = fromY; }
        void setToY(const uint32_t toY) { this->_toY = toY; }

    private:
        std::string _sceneContent;
        uint32_t _fromY;
        uint32_t _toY;
    };

}
