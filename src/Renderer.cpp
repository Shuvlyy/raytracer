#include "Renderer.hpp"
#include "Logger.hpp"

#include <format>

namespace raytracer
{

    Renderer::Renderer
    (
        const yml::Yml &yml
    )
        : _camera(Camera::fromConfig(yml)),
          _scene(yml)
    {
        yml::Node resolutionNode = yml["camera"]["resolution"];

        this->_width = resolutionNode["width"].as<int>();
        this->_height = resolutionNode["height"].as<int>();

        LOG_DEBUG("Renderer successfully loaded. (" + std::to_string(this->_width) + "x" + std::to_string(this->_height) + ")");
    }

    image::Ppm
    Renderer::render()
        const
    {
        image::Ppm image(this->_width, this->_height); // TODO: Change type of image according to user's choice (in a long time lol)

        const uint32_t totalPixels = this->_width * this->_height;

        for (uint32_t y = 0; y < this->_height; y++) {
            for (uint32_t x = 0; x < this->_width; x++) {
                const double u = static_cast<double>(x) / this->_width;
                const double v = static_cast<double>(y) / this->_height;

                const math::Ray ray = this->_camera.ray(u, v);

                // ...

                const uint32_t currentPixel = y * this->_width + x;
                const uint8_t progress = (static_cast<double>(currentPixel) / totalPixels) * 100;
                LOG_DEBUG(std::format(
                    "[{}%] Computed pixel ({}, {}).",
                    progress, x, y)
                );
            }
        }
        return image;
    }

}
