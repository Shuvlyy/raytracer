#include "Renderer.hpp"
#include "Logger.hpp"
#include "Math/Random.hpp"

#include <format>
#include <iostream>

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
        this->_antialiasingSamples = resolutionNode["antialiasingSamples"].as<int>();

        LOG_DEBUG(std::format(
            "Renderer successfully loaded. ({}x{}, {})",
            this->_width, this->_height, this->_antialiasingSamples
        ));
    }

    image::Ppm
    Renderer::render()
        const
    {
        image::Ppm image(this->_width, this->_height); // TODO: Change type of image according to user's choice (in a long time lol)

        const uint32_t totalPixels = this->_width * this->_height;

        for (uint32_t y = 0; y < this->_height; y++) {
            for (uint32_t x = 0; x < this->_width; x++) {
                math::Color pixelColor;

                for (int sample = 0; sample < this->_antialiasingSamples; sample++) {
                    double u = (x + math::randomDouble()) / this->_width;
                    double v = (y + math::randomDouble()) / this->_height;

                    const math::Ray ray = this->_camera.ray(u, v);
                    pixelColor += computeColor(ray);
                }

                image.setAt(
                    x, y,
                    1.0 / this->_antialiasingSamples * pixelColor
                );

                const uint32_t currentPixel = y * this->_width + x;
                const uint8_t progress = (currentPixel / totalPixels) * 100;
                std::clog << "\r[" << std::to_string(progress) << "%]" << std::flush;
            }
        }
        return image;
    }

    math::Color
    Renderer::computeColor
    (
        const math::Ray &ray
    )
        const
    {
        HitResult res{};

        if (this->_scene.hits(ray, res)) {
            return .5 * (res.n + math::Color(1.0, 1.0, 1.0));
        }

        math::Vec<3> dir = ray.direction.normalized();
        const double a = .5 * (dir[1] + 1);

        return (1.0 - a) * math::Color(1, 1, 1) + a * math::Color(.5, .7, 1);
    }

}
