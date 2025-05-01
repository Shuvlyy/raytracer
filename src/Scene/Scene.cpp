#include "Scene.hpp"

#include <iostream>

#include "Shape/Shapes/Sphere.hpp"
#include "Shape/Shapes/Plane.hpp"
#include "Shape/Material/Materials/Metal.hpp"
#include "Shape/Material/Materials/Lambertian.hpp"

#include "Logger.hpp"

namespace raytracer
{

    static
    math::Color
    getColorFromHex
    (
        std::string color
    )
    {
        int r = std::stoi(color.substr(3, 4), nullptr, 16);
        int g = std::stoi(color.substr(5, 6), nullptr, 16);
        int b = std::stoi(color.substr(7, 8), nullptr, 16);

        return math::Color(r / 255.0f, g / 255.0f, b / 255.0f);
    }

    static
    std::shared_ptr<shape::Material>
    getMaterial
    (
        yml::Tree shape
    )
    {
        auto mat = shape.getNodes().find("material");
        if (mat == shape.getNodes().end()) {
            return std::make_shared<shape::material::Lambertian>(getColorFromHex(shape["color"].as<>()));
        }
        if (mat->second.as<>() == "\"LAMBERTIAN\"") {
            return std::make_shared<shape::material::Lambertian>(getColorFromHex(shape["color"].as<>()));
        }
        return nullptr;
    }

    Scene::Scene
    (
        const yml::Yml &config
    ) : Scene() // FIXME: Temporary!!
    {
        const int nbShapes = config["shapes"].children.getNodes().size();
        for (int i = 0; i < nbShapes; i++) {
            const auto shape = config["shapes"][i].children;
            if (shape["type"].as<>() == "\"SPHERE\"") {
                this->_shapes.push_back(std::make_unique<shape::Sphere>(
                    math::Point<3>(shape["x"].as<int>(),
                                   shape["y"].as<int>(),
                                   shape["z"].as<int>()),
                    shape["radius"].as<double>(),
                    getMaterial(shape)
                ));
            }
            if (shape["type"].as<>() == "\"PLANE\"") {
                this->_shapes.push_back(std::make_unique<shape::Plane>(
                    math::Point<3>(0, 0, -20), // Axis: x, y, z (-20 for Z axi -- test.yml)
                    math::Vec<3>(0, 0, 1), // Vector: x, y, z (0, 0, 1 because axis is Z ?)
                    getMaterial(shape)
                ));
            }
            LOG_DEBUG("Added " + shape["type"].as<>() + " " + config["shapes"][i].name);
        }
        // ...
        LOG_DEBUG("Scene successfully loaded.");
    }

    // FIXME: Temporary!!
    Scene::Scene
    ()
    {
        // auto material_ground = std::make_shared<shape::material::Metal>(math::Color(0.8, 0.8, 0.0), 0);
        // auto material_center = std::make_shared<shape::material::Lambertian>(math::Color(0.1, 0.2, 0.5));
        // auto material_left   = std::make_shared<shape::material::Metal>(math::Color(1, 1, 1), 0);
        // auto material_right  = std::make_shared<shape::material::Metal>(math::Color(0.8, 0.6, 0.2), .5);
        //
        // auto earth = std::make_unique<shape::Sphere>(
        // math::Point<3>(0, -100.5, -1),
        //     100,
        //     material_ground
        // );
        //
        // auto ball1 = std::make_unique<shape::Sphere>(
        //     math::Point<3>(0, 0, -1.2),
        //     .5,
        //     material_center
        // );
        //
        // auto ball2 = std::make_unique<shape::Sphere>(
        //     math::Point<3>(-1, 0, -1),
        //     .5,
        //     material_left
        // );
        //
        // auto ball3 = std::make_unique<shape::Sphere>(
        //     math::Point<3>(1, 0, -1),
        //     .5,
        //     material_right
        // );
        //
        // this->_shapes.push_back(std::move(earth));
        // this->_shapes.push_back(std::move(ball1));
        // this->_shapes.push_back(std::move(ball2));
        // this->_shapes.push_back(std::move(ball3));
    }

    bool
    Scene::hits
    (
        const math::Ray &ray,
        HitResult& res
    )
        const
    {
        HitResult tmpRes;
        bool hasHit = false;
        double closest = ray.t_max;

        for (const auto& shape : this->_shapes) {
            math::Ray tmpRay = ray;
            tmpRay.t_max = closest;
            if (shape->hits(tmpRay, tmpRes)) {
                hasHit = true;
                closest = tmpRes.t;
                res = tmpRes;
            }
        }
        return hasHit;
    }

}
