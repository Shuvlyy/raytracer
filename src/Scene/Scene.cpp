#include "Scene.hpp"

#include "Factory/Factory.hpp"
#include "Shape/Shapes/Sphere.hpp"
#include "Shape/Shapes/Plane.hpp"
#include "Shape/Material/Materials/Metal.hpp"
#include "Shape/Material/Materials/Lambertian.hpp"
#include "Light/Lights/Point.hpp"
#include "Light/Lights/Directional.hpp"
#include "Light/Lights/Ambient.hpp"

#include "Logger.hpp"

namespace raytracer
{

    Scene::Scene
    (
        const yml::Yml &config
    ) : Scene()
    {
        size_t size = config["lights"].children.getNodes().size();
        for (size_t i = 0; i < size; i++) {
            const auto lightConfig = config["lights"][i];
            auto light = factory::getLightFromYml(lightConfig, this->_lights);
            if (!light || dynamic_cast<light::Point*>(light.get())) {
                continue;
            }
            this->_lights.push_back(std::move(light));
            LOG_DEBUG("Added " + lightConfig.name + " light.");
        }
        size = config["shapes"].children.getNodes().size();
        for (size_t i = 0; i < size; i++) {
            const auto shapeConfig = config["shapes"][i].children;
            auto shape = factory::getShapeFromYml(shapeConfig);
            if (shape == nullptr) {
                continue;
            }
            this->_shapes.push_back(std::move(shape));
            LOG_DEBUG("Added " + shapeConfig["type"].as<>() + " " + config["shapes"][i].name + ".");
        }
        LOG_DEBUG("Scene successfully loaded.");
    }

    // FIXME: Temporary!!
    Scene::Scene
    ()
    {
        // auto mirrorMaterial = std::make_shared<shape::material::Metal>(math::Color(1, 1, 1), 0);
        // auto ballMaterial = std::make_shared<shape::material::Lambertian>(math::Color(1, 0, 0));
        //
        // auto floor = std::make_unique<shape::Plane>(
        //     math::Point<3>(0, -1.5, 0),
        //     math::Vec<3>(0, 1, 0),
        //     mirrorMaterial
        // );
        //
        // auto ceil = std::make_unique<shape::Plane>(
        //     math::Point<3>(0, 1.5, 0),
        //     math::Vec<3>(0, -1, 0),
        //     mirrorMaterial
        // );
        //
        // auto left = std::make_unique<shape::Plane>(
        //     math::Point<3>(-1.5, 0, 0),
        //     math::Vec<3>(1, 0, 0),
        //     mirrorMaterial
        // );
        //
        // auto right = std::make_unique<shape::Plane>(
        //     math::Point<3>(1.5, 0, 0),
        //     math::Vec<3>(-1, 0, 0),
        //     mirrorMaterial
        // );
        //
        // auto forward = std::make_unique<shape::Plane>(
        //     math::Point<3>(0, 0, -2.5),
        //     math::Vec<3>(0, 0, 1),
        //     mirrorMaterial
        // );
        //
        // auto lightPoint = std::make_unique<light::Point>(
        //     math::Color(1, 1, 1),
        //     math::Vec<3>(1, 1, 1),
        //     20.0
        // );
        //
        // auto ball1 = std::make_unique<shape::Sphere>(
        //     math::Point<3>(0, 0, -1),
        //     .3,
        //     ballMaterial
        // );
        //
        // this->_shapes.push_back(std::move(floor));
        // this->_shapes.push_back(std::move(ceil));
        // this->_shapes.push_back(std::move(left));
        // this->_shapes.push_back(std::move(right));
        // this->_shapes.push_back(std::move(forward));
        // this->_shapes.push_back(std::move(ball1));
        //
        // this->_lights.push_back(std::move(lightPoint));



        auto material_ground = std::make_shared<shape::material::Lambertian>(math::Color(0.8, 0.8, 0.2));
        auto material_ceil = std::make_shared<shape::material::Metal>(math::Color(0.8, 0.8, 0.8), .03);
        auto material_center = std::make_shared<shape::material::Lambertian>(math::Color(0.1, 0.2, 0.5));
        auto material_left   = std::make_shared<shape::material::Metal>(math::Color(1, 1, 1), 0);

        auto earth = std::make_unique<shape::Plane>(
            math::Point<3>(0, -.5, 0),
            math::Vec<3>(0, 1, 0),
            material_ground
        );

        auto earth2 = std::make_unique<shape::Plane>(
            math::Point<3>(0, .5, 0),
            math::Vec<3>(0, -1, 0),
            material_ceil
        );

        auto earth3 = std::make_unique<shape::Plane>(
            math::Point<3>(0, 0, -8),
            math::Vec<3>(0, 0, 1),
            material_ceil
        );

        auto ball1 = std::make_unique<shape::Sphere>(
            math::Point<3>(0, 0, -1.2),
            .5,
            material_center
        );

        // auto ball2 = std::make_unique<shape::Sphere>(
        //     math::Point<3>(-1, 0, -1),
        //     .5,
        //     material_left
        // );

        for (double k = -1; k > -15; k--) {
            auto b = std::make_unique<shape::Sphere>(
                math::Point<3>(k, 0, -1),
                .3 + (-k / 10),
                material_left
            );

            this->_shapes.push_back(std::move(b));
        }

        auto light = std::make_unique<light::Directional>(
            math::Color(.9, .9, .9),
            math::Vec<3>(0, -1, -1)
        );

        auto light3 = std::make_unique<light::Point>(
            math::Color(1, 1, 1),
            math::Vec<3>(1, 0, -1.4),
            100.0
        );

        auto ambientLight = std::make_unique<light::Ambient>(
            math::Color(1, 1, 1),
            .2
        );

        // auto light2 = std::make_unique<light::Point>(
        //     math::Color(.71, .38, .75),
        //     math::Vec<3>(-1, 0, 0),
        //     3.0
        // );

        this->_shapes.push_back(std::move(earth));
        // this->_shapes.push_back(std::move(earth3));
        // this->_shapes.push_back(std::move(earth2));
        this->_shapes.push_back(std::move(ball1));
        // this->_shapes.push_back(std::move(ball2));

        // this->_lights.push_back(std::move(light));
        this->_lights.push_back(std::move(light3));
        this->_lights.push_back(std::move(ambientLight));
        // this->_lights.push_back(std::move(light2));


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
