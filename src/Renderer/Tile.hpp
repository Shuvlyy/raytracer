#pragma once

#include <atomic>

namespace raytracer::renderer
{

    struct Tile
    {
        uint32_t x, y;
        uint32_t width, height;
        std::atomic<int> threadNumber;

        Tile
        (
            const uint32_t x, const uint32_t y,
            const uint32_t width, const uint32_t height
        ) : x(x), y(y), width(width), height(height), threadNumber(-1) {}

        Tile() : x(0), y(0), width(0), height(0), threadNumber(-1) {}

        Tile(const Tile& other)
            : x(other.x), y(other.y), width(other.width), height(other.height),
              threadNumber(other.threadNumber.load()) {}

        Tile(Tile&& other) noexcept
            : x(other.x), y(other.y), width(other.width), height(other.height),
              threadNumber(other.threadNumber.load()) {}

        bool
        operator==(const Tile& other) const
        {
            return
                x == other.x && y == other.y &&
                width == other.width && height == other.height &&
                threadNumber == other.threadNumber;
        }

        Tile&
        operator=(const Tile& other)
        {
            if (this == &other) {
                return *this;
            }

            x = other.x;
            y = other.y;
            width = other.width;
            height = other.height;
            threadNumber.store(other.threadNumber.load());
            return *this;
        }

        Tile&
        operator=(Tile&& other) noexcept
        {
            if (*this == other) {
                return *this;
            }

            x = other.x;
            y = other.y;
            width = other.width;
            height = other.height;
            threadNumber.store(other.threadNumber.load());
            return *this;
        }
    };

}
