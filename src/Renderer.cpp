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
          _scene(yml),
          _settings{}
    {
        yml::Node resolutionNode = yml["camera"]["resolution"];

        this->_width = resolutionNode["width"].as<int>();
        this->_height = resolutionNode["height"].as<int>();
        this->_settings.antiAliasingSamples = resolutionNode["antialiasingSamples"].as<int>();
        this->_settings.maxBounces = resolutionNode["maxBounces"].as<int>();

        LOG_DEBUG(std::format(
            "Renderer successfully loaded. ({}x{}, {}, Maximum bounces: {})",
            this->_width, this->_height,
            this->_settings.antiAliasingSamples, this->_settings.maxBounces
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

                for (size_t spl = 0; spl < this->_settings.antiAliasingSamples; spl++) {
                    double u = (x + math::randomDouble()) / this->_width;
                    double v = (y + math::randomDouble()) / this->_height;

                    const math::Ray ray = this->_camera.ray(u, v);
                    pixelColor += computeColor(ray, this->_settings.maxBounces);
                }

                image.setAt(x, y,
                    1.0
                    / static_cast<double>(this->_settings.antiAliasingSamples)
                    * pixelColor
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
        const math::Ray &ray,
        size_t bounces
    )
        const
    {
        if (bounces == 0) {
            return {}; // Pure black
        }

        HitResult res{};

        if (this->_scene.hits(ray, res)) {
            math::Vec<3> bounce = res.n + getRandomUnitVector();

            if (dot(bounce, res.n) <= 0.0) {
                bounce *= -1;
            }
            return .5 * computeColor(math::Ray(res.p, bounce), bounces--);
        }

        math::Vec<3> dir = ray.direction.normalized();
        const double a = .5 * (dir[1] + 1);

        return (1.0 - a) * math::Color(1, 1, 1) + a * math::Color(.5, .7, 1);
    }

    math::Vec<3>
    Renderer::getRandomUnitVector()
    {
        while (true) {
            auto p = math::Vec<3>::random(-1, 1);
            auto len = p.length() * p.length();

            if (len <= 1) {
                return p / std::sqrt(len);
            }
        }
    }
}
