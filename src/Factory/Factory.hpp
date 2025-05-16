#pragma once

#include "yml/Yml.hpp"
#include "Shape/Shape.hpp"
#include "Light/Light.hpp"
#include "Shape/Material/Material.hpp"

namespace raytracer::factory
{

    std::unique_ptr<Shape> getShapeFromYml(
        const yml::Tree &shape,
        std::vector<std::unique_ptr<Shape>> &shapes);

    std::unique_ptr<Light> getLightFromYml(
        const yml::Node &light,
        std::vector<std::unique_ptr<Light>> &lights
    );

    void FromObj(
        const std::string &filepath,
        std::shared_ptr<shape::Material> material,
        std::vector<std::unique_ptr<Shape>> &shapes
    );

}

