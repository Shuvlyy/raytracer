#include "Scene.hpp"

#include <iostream>

#include "Factory/Factory.hpp"

#include "logger/Logger.hpp"
#include "Light/Lights/Point.hpp"

#include "Shape/Material/Materials/Lambertian.hpp"
#include "Shape/Material/Materials/Metal.hpp"
#include "Shape/Shapes/Sphere.hpp"

namespace raytracer
{

    Scene::Scene
    (
        const yml::Yml &config
    )
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
