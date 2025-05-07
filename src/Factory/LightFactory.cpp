#include <memory>

#include "Logger.hpp"
#include "Light/Lights/Ambient.hpp"
#include "Light/Lights/Directional.hpp"
#include "Light/Lights/Point.hpp"
#include "Math/Point.hpp"

#include "yml/Yml.hpp"

namespace raytracer::factory {

    std::unique_ptr<Light>
    getLightFromYml
    (
        const yml::Node &light,
        std::vector<std::unique_ptr<Light>> &lights
    )
    {
        if (light.name == AMBIENT) {
            return std::make_unique<light::Ambient>(
                math::getColor(light),
                light["intensity"].as<double>());
        }
        if (light.name == DIRECTIONAL) {
            return std::make_unique<light::Directional>(
                    math::getColor(light),
                    math::getVector3(light["direction"].children),
                    1);
        }
        if (light.name == POINTS) {
            for (size_t i = 0; i < light.children.getNodes().size(); i++) {
                auto point = light.children[i];
                lights.push_back(std::make_unique<light::Point>(
                    math::getColor(point),
                    math::Point(math::getVector3(point.children)),
                    point["intensity"].as<double>()
                ));
                LOG_DEBUG("Added light point " + point.name + ".");
            }
            return std::make_unique<light::Point>(math::Color(0, 0, 0), math::Vec<3>(0, 0, 0), 0);
        }
        LOG_WARN("Unknown light " + light.name + " ignoring.");
        return nullptr;
    }

}
