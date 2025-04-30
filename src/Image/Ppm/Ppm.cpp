#include "Ppm.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

namespace raytracer::image
{

    Ppm::Ppm
    (
        const uint32_t width,
        const uint32_t height
    ) : Image(width, height) {}

    void Ppm::save
    (
        const std::string &filepath
    )
        const
    {
        const uint32_t width = this->getDimensions()[0];
        const uint32_t height = this->getDimensions()[1];

        std::ostringstream oss;

        oss << "P3\n"
            << std::to_string(width) << " " << std::to_string(height) << "\n"
            << std::to_string(std::numeric_limits<uint8_t>::max() - 1) << "\n";

        for (uint32_t y = 0; y < height; ++y) {
            for (uint32_t x = 0; x < width; ++x) {
                math::Color color = this->at(x, y);

                oss << std::to_string(static_cast<uint8_t>(color[0] * 255)) << " "
                    << std::to_string(static_cast<uint8_t>(color[1] * 255)) << " "
                    << std::to_string(static_cast<uint8_t>(color[2] * 255)) << "\n";
            }
        }

        std::ofstream file(filepath, std::ios::out);

        file << oss.str();
        file.close();
    }

}
