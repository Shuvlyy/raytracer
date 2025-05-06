#include "Logger.hpp"
#include "Math/Color.hpp"

namespace raytracer::math {

    Color
    getColor
    (
        const yml::Tree &shape
    ) {
        if (!shape.getNodes().contains("color")) {
            LOG_WARN("No color specified, falling back to white");
            return Color(1, 1, 1);
        }
        const auto color = shape["color"].as<>();

        const long r = std::stol(color.substr(3, 4), nullptr, 16);
        const long g = std::stol(color.substr(5, 6), nullptr, 16);
        const long b = std::stol(color.substr(7, 8), nullptr, 16);

        return Color(r / 255.0f, g / 255.0f, b / 255.0f);
    }

    Color
    getColor
    (
        const yml::Node &light
    )
    {
        if (!light.children.getNodes().contains("color")) {
            LOG_WARN("No color specified, falling back to white");
            return Color(1, 1, 1);
        }
        const auto color = light["color"].as<>();

        const long r = std::stol(color.substr(3, 4), nullptr, 16);
        const long g = std::stol(color.substr(5, 6), nullptr, 16);
        const long b = std::stol(color.substr(7, 8), nullptr, 16);

        return Color(r / 255.0f, g / 255.0f, b / 255.0f);
    }
}
