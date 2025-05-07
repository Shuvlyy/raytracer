#include "Logger.hpp"
#include "Math/Color.hpp"
#include "yml/Yml.hpp"

#include "Shape/Shapes/Sphere.hpp"
#include "Shape/Shapes/Plane.hpp"
#include "Shape/Material/Materials/Metal.hpp"
#include "Shape/Material/Materials/Lambertian.hpp"

namespace raytracer::shape::material
{

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
            double fuzz = 0;
            if (shape.getNodes().contains("fuzz")) {
                fuzz = shape["fuzz"].as<double>();
            } else {
                LOG_WARN("No fuzz specified, falling back to 0 (perfect mirror).");
            }
            return std::make_shared<Metal>(math::getColor(shape), fuzz);
        }
        LOG_WARN("Unknown material: " + shape["material"].as<>() + " falling back to lambertian.");
        return std::make_shared<Lambertian>(math::getColor(shape));;
    }

}

namespace raytracer::factory
{

    std::unique_ptr<Shape>
    getShapeFromYml
    (
        const yml::Tree &shape
    )
    {
        const auto shapeType = shape["type"].as<std::string>();

        if (shapeType == SPHERE) {
            return std::make_unique<shape::Sphere>(
                math::Point(math::getVector3(shape)),
                shape["radius"].as<double>(),
                shape::material::getMaterial(shape)
            );
        }
        if (shapeType == PLANE) {
            const auto axis = shape["axis"].as<std::string>();
            const auto pos = shape["position"].as<double>();
            const auto axisPos = pos >= 0 ? -1 : 1;

            if (axis == X_AXIS) {
                return std::make_unique<shape::Plane>(
                    math::Point<3>(pos, 0, 0),
                    math::Vec<3>(axisPos, 0, 0),
                    shape::material::getMaterial(shape)
                );
            }
            if (axis == Y_AXIS) {
                return std::make_unique<shape::Plane>(
                    math::Point<3>(0, pos, 0),
                    math::Vec<3>(0, axisPos, 0), // TODO: Use a function to get the Vec<3>
                    shape::material::getMaterial(shape)
                );
            }
            if (axis == Z_AXIS) {
                return std::make_unique<shape::Plane>(
                    math::Point<3>(0, 0, pos),
                    math::Vec<3>(0, 0, axisPos),
                    shape::material::getMaterial(shape)
                );
            }
        }
        LOG_WARN("Unknown shape: \"" + shapeType + "\". Ignoring...");
        return nullptr;
    }

}
