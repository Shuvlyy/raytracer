#pragma once

#include "Camera/Camera.hpp"
#include "Scene/Scene.hpp"
#include "yml/Yml.hpp"
#include "Image/Ppm/Ppm.hpp"

namespace raytracer
{

    class Renderer final
    {
    public:
        explicit Renderer(const yml::Yml& yml);

        image::Ppm render() const; // TODO: Any type of image

    private:
        uint32_t _width, _height;
        Camera _camera;
        Scene _scene;

        [[nodiscard]] math::Color computeColor(const math::Ray& ray) const;
    };

}
