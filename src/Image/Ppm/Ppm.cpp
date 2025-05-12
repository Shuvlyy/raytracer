#include "Ppm.hpp"

#include <sstream>
#include <fstream>

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
            << std::to_string(MAX_COLOR_SHADES - 1) << "\n";

        for (uint32_t y = 0; y < height; ++y) {
            for (uint32_t x = 0; x < width; ++x) {
                math::Color color = this->at(x, y);

                oss << std::to_string(static_cast<uint16_t>(color[0] * MAX_COLOR_SHADES)) << " "
                    << std::to_string(static_cast<uint16_t>(color[1] * MAX_COLOR_SHADES)) << " "
                    << std::to_string(static_cast<uint16_t>(color[2] * MAX_COLOR_SHADES)) << "\n";
            }
        }

        std::ofstream file(filepath + PPM_EXTENSION, std::ios::out);

        file << oss.str();
        file.close();
    }

}
