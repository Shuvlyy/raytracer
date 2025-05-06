#pragma once

#include "yml/Yml.hpp"
#include "Shape/Shape.hpp"

namespace raytracer::factory {

    std::unique_ptr<Shape> getShapeFromYml(const yml::Tree &shape);
    std::unique_ptr<Light> getLightFromYml (
        const yml::Node &light,
        std::vector<std::unique_ptr<Light>> &lights
    );

}

