#pragma once

#include "yml/Yml.hpp"
#include "Shape/Shape.hpp"
#include "Light/Light.hpp"

#include <vector>
#include <memory>

namespace raytracer
{

    class Scene final
    {
    public:
        explicit Scene(const yml::Yml &config);
        explicit Scene();

        [[nodiscard]] bool hits(const math::Ray& ray, HitResult& res) const;

        [[nodiscard]] const std::vector<std::unique_ptr<Shape>>& getShapes()
            const { return this->_shapes; }
        [[nodiscard]] const std::vector<std::unique_ptr<Light>>& getLights()
            const { return this->_lights; }

    private:
        std::vector<std::unique_ptr<Shape>> _shapes;
        std::vector<std::unique_ptr<Light>> _lights;
    };

}
