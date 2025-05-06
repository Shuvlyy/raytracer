#include "Logger.hpp"
#include "Math/Color.hpp"
#include "yml/Yml.hpp"

#include "Shape/Shapes/Sphere.hpp"
#include "Shape/Shapes/Plane.hpp"
#include "Shape/Material/Materials/Metal.hpp"
#include "Shape/Material/Materials/Lambertian.hpp"

namespace raytracer::shape::material {

    static
    std::shared_ptr<Material>
    getMaterial
    (
        const yml::Tree &shape
    )
    {
        if (!shape.getNodes().contains("material")) {
            LOG_WARN("No material specified, falling back to lambertian.");
            return std::make_shared<Lambertian>(math::getColor(shape));
        }
        if (shape["material"].as<>() == LAMBERTIAN) {
            return std::make_shared<Lambertian>(math::getColor(shape));
        }
        if (shape["material"].as<>() == METAL) {
            long fuzz = 0;
            if (shape.getNodes().contains("fuzz")) {
                fuzz = shape["fuzz"].as<long>();
            } else {
                fuzz = 0;
                LOG_WARN("No fuzz specified, falling back to 0 (perfect mirror).");
            }
            return std::make_shared<Metal>(math::getColor(shape), fuzz);
        }
        LOG_WARN("Unknown material: " + shape["material"].as<>() + " falling back to lambertian.");
        return std::make_shared<Lambertian>(math::getColor(shape));;
    }

}

namespace raytracer::factory {

    std::unique_ptr<Shape>
    getShapeFromYml
    (
        const yml::Tree &shape
    )
    {
        if (shape["type"].as<>() == SPHERE) {
            return std::make_unique<shape::Sphere>(
                math::Point<3>(shape["x"].as<int>(),
                               shape["y"].as<int>(),
                               shape["z"].as<int>()),
                shape["radius"].as<double>(),
                shape::material::getMaterial(shape));
        }
        if (shape["type"].as<>() == PLANE) {
            if (shape["axis"].as<>() == X_AXIS) {
                return std::make_unique<shape::Plane>(
                    math::Point<3>(shape["position"].as<int>(), 0, 0),
                    math::Vec<3>(1, 0, 0),
                    shape::material::getMaterial(shape));
            }
            if (shape["axis"].as<>() == Y_AXIS) {
                return std::make_unique<shape::Plane>(
                    math::Point<3>(0, shape["position"].as<int>(), 0),
                    math::Vec<3>(0, 1, 0),
                    shape::material::getMaterial(shape));
            }
            if (shape["axis"].as<>() == Z_AXIS) {
                return std::make_unique<shape::Plane>(
                    math::Point<3>(0, 0, shape["position"].as<int>()),
                    math::Vec<3>(0, 0, 1),
                    shape::material::getMaterial(shape));
            }
        }
        LOG_WARN("Unknown shape: " + shape["type"].as<>() + " ignoring.");
        return nullptr;
    }

}
