#include "Scene.hpp"

#include "Shape/Shapes/Sphere/Sphere.hpp"
#include "Shape/Material/Materials/Metal.hpp"
#include "Shape/Material/Materials/Lambertian.hpp"

#include "Logger.hpp"

namespace raytracer
{

    Scene::Scene
    (
        const yml::Yml &config
    ) : Scene() // FIXME: Temporary!!
    {
        // ...
        LOG_DEBUG("Scene successfully loaded.");
    }

    // FIXME: Temporary!!
    Scene::Scene
    ()
    {
        auto material_ground = std::make_shared<shape::material::Lambertian>(math::Color(0.8, 0.8, 0.0));
        auto material_center = std::make_shared<shape::material::Lambertian>(math::Color(0.1, 0.2, 0.5));
        auto material_left   = std::make_shared<shape::material::Metal>(math::Color(1, 1, 1));
        auto material_right  = std::make_shared<shape::material::Metal>(math::Color(0.8, 0.6, 0.2));

        auto earth = std::make_unique<shape::Sphere>(
            math::Point<3>(0, -100.5, -1),
            100,
            material_ground
        );

        auto ball1 = std::make_unique<shape::Sphere>(
            math::Point<3>(0, 0, -1.2),
            .5,
            material_center
        );

        auto ball2 = std::make_unique<shape::Sphere>(
            math::Point<3>(-1, 0, -1),
            .5,
            material_left
        );

        auto ball3 = std::make_unique<shape::Sphere>(
            math::Point<3>(1, 0, -1),
            .5,
            material_right
        );

        this->_shapes.push_back(std::move(earth));
        this->_shapes.push_back(std::move(ball1));
        this->_shapes.push_back(std::move(ball2));
        this->_shapes.push_back(std::move(ball3));
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
