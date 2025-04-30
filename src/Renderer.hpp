#pragma once

#include "Camera/Camera.hpp"
#include "Scene/Scene.hpp"
#include "yml/Yml.hpp"
#include "Image/Ppm/Ppm.hpp"

namespace raytracer
{

    namespace renderer
    {

        struct Settings
        {
            size_t antiAliasingSamples;
            size_t maxBounces;
        };

    }

    class Renderer final
    {
    public:
        explicit Renderer(const yml::Yml& yml);

        [[nodiscard]] image::Ppm render() const; // TODO: Any type of image

    private:
        uint32_t _width, _height;
        Camera _camera;
        Scene _scene;
        renderer::Settings _settings;

        [[nodiscard]] math::Color computeColor(
            const math::Ray& ray,
            size_t bounces
        ) const;

        [[nodiscard]] math::Color computeDirectLighting(
            const HitResult& res
        ) const;
    };

}
