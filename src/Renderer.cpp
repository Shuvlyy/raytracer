#include "Renderer.hpp"
#include "Logger.hpp"
#include "Math/Random.hpp"
#include "Shape/Material/Material.hpp"

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

        this->_render = std::make_unique<image::Ppm>(this->_width, this->_height);

        LOG_DEBUG(std::format(
            "Renderer successfully loaded. ({}x{}, {}, Maximum bounces: {})",
            this->_width, this->_height,
            this->_settings.antiAliasingSamples, this->_settings.maxBounces
        ));
    }

    void
    Renderer::render
    (
        const uint32_t fromY,
        const uint32_t toY
    )
        const
    {
        if (fromY > this->_height) {
            LOG_FATAL("fromY is out of bounds.");
            return;
        }

        if (toY > this->_height) {
            LOG_FATAL("toY is out of bounds.");
            return;
        }

        LOG_INFO("Starting rendering...");

        const uint32_t totalPixels = this->_width * this->_height;

        for (uint32_t y = fromY; y < toY; y++) {
            for (uint32_t x = 0; x < this->_width; x++) {
                math::Color pixelColor;

                for (size_t spl = 0; spl < this->_settings.antiAliasingSamples; spl++) {
                    const double u = (x + math::randomDouble()) / this->_width;
                    const double v = (y + math::randomDouble()) / this->_height;

                    math::Ray ray = this->_camera.ray(u, v);
                    pixelColor += computeColor(ray, this->_settings.maxBounces);
                }

                pixelColor *= 1.0 / this->_settings.antiAliasingSamples;
                pixelColor.clamp(0.0, 1.0);

                this->_render->setAt(x, y, pixelColor);

                const uint32_t currentPixel = y * this->_width + x;
                const auto progress = static_cast<uint8_t>(
                    static_cast<double>(currentPixel) /
                    static_cast<double>(totalPixels) * 100
                );
                std::clog << "\r[" << std::to_string(progress) << "%]" << std::flush;
            }
        }

        LOG_INFO("Render finished.");
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
            math::Ray scattered;
            math::Color attenuation;

            if (res.material->scatter(ray, res, attenuation, scattered)) {
                const math::Color indirectLighting = computeColor(scattered, bounces - 1);
                const math::Color directLighting = computeDirectLighting(res);

                return attenuation * (indirectLighting + directLighting);
            }
            return {}; // Pure black
        }

        return {};

        // math::Vec<3> dir = ray.direction.normalized();
        // const double a = .5 * (dir[1] + 1);

        // return (1.0 - a) * math::Color(1, 1, 1) + a * math::Color(.5, .7, 1);
    }

    math::Color
    Renderer::computeDirectLighting
    (
        const HitResult &res
    )
        const
    {
        math::Color color;

        for (auto& light : this->_scene.getLights()) {
            math::Vec<3> directionToLight;
            double distanceToLight;
            const math::Color lightColor = light->sample(res.p, directionToLight, distanceToLight);

            if (light->isAmbient()) {
                color += lightColor;
                continue;
            }

            math::Ray shadowRay(res.p + 1e-4 * directionToLight, directionToLight);
            HitResult shadowHit;

            if (this->_scene.hits(shadowRay, shadowHit)) {
                if (shadowHit.t < distanceToLight) {
                    continue; // Blocked by an object
                }
            }

            const double nDotL = std::max(dot(res.n, directionToLight), 0.0);
            color += lightColor * nDotL;
        }
        return color;
    }

}
