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
            uint32_t x = 0,
            uint32_t y = 0,
            uint32_t width = 0,
            uint32_t height = 0
        );

        [[nodiscard]] ByteBuffer serialize() const override;
        void deserialize(const uint8_t* data, size_t size) override;

        [[nodiscard]] std::string getSceneContent() const { return this->_sceneContent; }
        [[nodiscard]] uint32_t getX() const { return this->_x; }
        [[nodiscard]] uint32_t getY() const { return this->_y; }
        [[nodiscard]] uint32_t getWidth() const { return this->_width; }
        [[nodiscard]] uint32_t getHeight() const { return this->_height; }

    private:
        std::string _sceneContent;
        uint32_t _x, _y, _width, _height;
    };

}
