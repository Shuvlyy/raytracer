#include "Scene.hpp"

#include "Shape/Shapes/Sphere/Sphere.hpp"

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
        auto ball1 = std::make_unique<shape::Sphere>(
            math::Point<3>(0, 0, -1),
            .5,
            nullptr
        );

        auto ball2 = std::make_unique<shape::Sphere>(
            math::Point<3>(0, -100.5, -1),
            100,
            nullptr
        );

        this->_shapes.push_back(std::move(ball1));
        this->_shapes.push_back(std::move(ball2));
    }

    bool
    Scene::hits
    (
        const math::Ray &ray,
        HitResult& res
    )
        const
    {
        for (const auto& shape : this->_shapes) {
            if (shape->hits(ray, res)) {
                return true;
            }
        }
        res.t = -1;
        return false;
    }

}
