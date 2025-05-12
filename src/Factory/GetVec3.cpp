#include <iostream>

#include "logger/Logger.hpp"
#include "Math/Vec.hpp"

namespace raytracer::math {

    Vec<3>
    getVector3
    (
        const yml::Tree &config
    ) {
        Vec<3> res;
        bool changed = false;

        if (config.getNodes().contains("x")) {
            res += Vec<3>(config["x"].as<double>(), 0, 0);
            changed = true;
        }
        if (config.getNodes().contains("y")) {
            res += Vec<3>(0, config["y"].as<double>(), 0);
            changed = true;
        }
        if (config.getNodes().contains("z")) {
            res += Vec<3>(0, 0, config["z"].as<double>());
            changed = true;
        }

        if (!changed) {
            LOG_WARN("No coordinates specified, falling back to (0, 0, 0).");
        }
        return res;
    }


}