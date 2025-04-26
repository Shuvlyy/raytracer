#include "Camera.hpp"

namespace raytracer
{

    Camera::Camera(
        const camera::Settings& settings
    )
    {
        const double aspectRatio =
            double(settings.width) / double(settings.height);

        const double theta = settings.fov * std::numbers::pi / 180.0;
        const double halfHeight = std::tan(theta / 2.0);
        const double halfWidth = aspectRatio * halfHeight;

        const math::Point<3> lowerLeft =
            origin + math::Vec<3>(-halfWidth, -halfHeight, -1);
        const auto horizontal = math::Vec<3>(2 * halfWidth, 0, 0);
        const auto vertical = math::Vec<3>(0, 2 * halfHeight, 0);

        this->screen = math::Rect<3>(
            lowerLeft,
            horizontal,
            vertical
        );
    }

    Camera
    Camera::fromConfig
    (
        const yml::Yml &config
    )
    {
        yml::Node cameraConfig = config["camera"];

        camera::Settings settings;
        settings.width = cameraConfig["resolution"]["width"].as<int>();
        settings.height = cameraConfig["resolution"]["height"].as<int>();
        settings.position = math::Vec<3>(
            cameraConfig["position"]["x"].as<int>(),
            cameraConfig["position"]["y"].as<int>(),
            cameraConfig["position"]["z"].as<int>()
        );
        settings.rotation = math::Vec<3>(
            cameraConfig["rotation"]["x"].as<int>(),
            cameraConfig["rotation"]["y"].as<int>(),
            cameraConfig["rotation"]["z"].as<int>()
        );
        settings.fov = cameraConfig["fov"].as<double>();

        return Camera(settings);
    }

    math::Ray
    Camera::ray
    (
        const double u,
        const double v
    )
        const
    {
        const math::Point<3> screenPoint = this->screen.at(u, v);

        return math::Ray(
            this->origin,
            math::Point<3>::toVec(this->origin, screenPoint)
        );
    }

}
