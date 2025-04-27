#pragma once

#include "Shape/Shape.hpp"
#include "yml/Yml.hpp"

#include <vector>
#include <memory>

namespace raytracer
{

    class Scene final
    {
    public:
        explicit Scene(const yml::Yml &config);
        explicit Scene();

        [[nodiscard]] bool hits(const math::Ray& ray) const;

    private:
        std::vector<std::unique_ptr<Shape>> _shapes;
    };

}
