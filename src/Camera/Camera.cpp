#define DEG_TO_RAD(x)   x * std::numbers::pi / 180.0;

#include "Camera.hpp"
#include "Logger.hpp"
#include "Math/Random.hpp"
#include "Math/Mat.hpp"

#include <sstream>
#include <iomanip>

namespace raytracer
{

    Camera::Camera(
        const camera::Settings& settings
    )
    {
        this->origin = settings.position;
        this->lensRadius = settings.aperture / 2;

        const double aspectRatio =
            static_cast<double>(settings.width) /
            static_cast<double>(settings.height);
        const double theta = DEG_TO_RAD(settings.fov);
        const double halfHeight = std::tan(theta / 2.0);
        const double halfWidth = aspectRatio * halfHeight;

        const double yaw = DEG_TO_RAD(settings.rotation[0]);
        const double pitch = DEG_TO_RAD(settings.rotation[1]);
        const double roll = DEG_TO_RAD(settings.rotation[2]);

        const auto rotationMat = math::Mat<3>::fromEuler(yaw, pitch, roll);

        math::Vec<3> forward(0, 0, -1);
        math::Vec<3> up(0, -1, 0);
        math::Vec<3> right(1, 0, 0);

        forward = rotationMat * forward;
        up = rotationMat * up;
        right = rotationMat * right;

        const math::Point<3> lowerLeft = origin - halfWidth * right - halfHeight * up + forward;

        const auto horizontal = 2 * halfWidth * right;
        const auto vertical = 2 * halfHeight * up;

        this->screen = math::Rect<3>(
            lowerLeft,
            horizontal,
            vertical
        );

        LOG_DEBUG("Camera successfully loaded.");

        std::stringstream logMessage;
        logMessage << std::fixed << std::setprecision(1)
                   << "Camera settings: "
                   << settings.width << "x" << settings.height
                   << " @ (" << settings.position[0] << "," << settings.position[1] << "," << settings.position[2] << "),"
                   << "(" << settings.rotation[0] << "," << settings.rotation[1] << "," << settings.rotation[2] << ") "
                   << settings.fov << "°";

        LOG_DEBUG(logMessage.str());
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
        settings.position = math::Point<3>(
            cameraConfig["position"]["x"].as<double>(),
            cameraConfig["position"]["y"].as<double>(),
            cameraConfig["position"]["z"].as<double>()
        );
        settings.rotation = math::Vec<3>(
            cameraConfig["rotation"]["yaw"].as<double>(),
            cameraConfig["rotation"]["pitch"].as<double>(),
            cameraConfig["rotation"]["roll"].as<double>()
        );
        settings.fov = cameraConfig["fov"].as<double>();
        settings.aperture = cameraConfig["aperture"].as<double>();
        settings.focalLength = cameraConfig["focalLength"].as<double>();

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
        return math::Ray(
            this->origin,
            math::Point<3>::toVec(
                this->origin,
                this->screen.at(u, v)
            )
        );
    }

}
