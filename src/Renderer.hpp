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
        explicit Renderer();

        static Renderer fromConfig(const yml::Yml& yml) {return Renderer(yml);}

        void render(
            uint32_t x, uint32_t y,
            uint32_t width, uint32_t height
        ) const;

        [[nodiscard]] uint32_t getWidth() const { return this->_width; }
        [[nodiscard]] uint32_t getHeight() const { return this->_height; }
        [[nodiscard]] const std::unique_ptr<Image>& getRender() const { return this->_render; }

    private:
        uint32_t _width, _height;
        Camera _camera;
        Scene _scene;
        renderer::Settings _settings;
        std::unique_ptr<Image> _render;

        [[nodiscard]] math::Color computeColor(
            const math::Ray& ray,
            size_t bounces
        ) const;

        [[nodiscard]] math::Color computeDirectLighting(
            const HitResult& res
        ) const;
    };

}
