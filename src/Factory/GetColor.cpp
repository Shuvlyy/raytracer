#include <iostream>

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

        const double r = std::stoi(color.substr(2, 2), nullptr, 16);
        const double g = std::stoi(color.substr(4, 2), nullptr, 16);
        const double b = std::stoi(color.substr(6, 2), nullptr, 16);

        return Color(r / 255.0, g / 255.0, b / 255.0);
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

        const double r = std::stoi(color.substr(2, 2), nullptr, 16);
        const double g = std::stoi(color.substr(4, 2), nullptr, 16);
        const double b = std::stoi(color.substr(6, 2), nullptr, 16);

        return Color(r / 255.0, g / 255.0, b / 255.0);
    }
}
