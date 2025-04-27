#define DEG_TO_RAD(x)   x * std::numbers::pi / 180.0;

#include "Camera.hpp"
#include "Logger.hpp"

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

        math::Vec<3> forward(0, 0, -1); // -Z is down
        math::Vec<3> up(0, 1, 0);       // +Y is up
        math::Vec<3> right(1, 0, 0);    // +X is right

        // Apply yaw (Y axis rotation)
        {
            const double cosYaw= std::cos(yaw);
            const double sinYaw = std::sin(yaw);

            forward = math::Vec<3>(
                cosYaw * forward[0] + sinYaw * forward[2],
                forward[1],
                -sinYaw * forward[0] + cosYaw * forward[2]
            );

            right = math::Vec<3>(
                cosYaw * right[0] + sinYaw * right[2],
                right[1],
                -sinYaw * right[0] + cosYaw * right[2]
            );
        }

        // Apply pitch (X axis rotation)
        {
            const double cosPitch = std::cos(pitch);
            const double sinPitch = std::sin(pitch);

            forward = math::Vec<3>(
                forward[0],
                cosPitch * forward[1] - sinPitch * forward[2],
                sinPitch * forward[1] + cosPitch * forward[2]
            );

            up = math::Vec<3>(
                up[0],
                cosPitch * up[1] - sinPitch * up[2],
                sinPitch * up[1] + cosPitch * up[2]
            );
        }

        // Apply roll (Z axis rotation)
        {
            const double cosRoll = std::cos(roll);
            const double sinRoll = std::sin(roll);

            right = math::Vec<3>(
                cosRoll * right[0] - sinRoll * right[1],
                sinRoll * right[0] + cosRoll * right[1],
                right[2]
            );

            up = math::Vec<3>(
                cosRoll * up[0] - sinRoll * up[1],
                sinRoll * up[0] + cosRoll * up[1],
                up[2]
            );
        }

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
            cameraConfig["position"]["x"].as<int>(),
            cameraConfig["position"]["y"].as<int>(),
            cameraConfig["position"]["z"].as<int>()
        );
        settings.rotation = math::Vec<3>(
            cameraConfig["rotation"]["yaw"].as<int>(),
            cameraConfig["rotation"]["pitch"].as<int>(),
            cameraConfig["rotation"]["roll"].as<int>()
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
        const math::Point<3> screenPoint = this->screen.at(u, v);

        return math::Ray(
            this->origin,
            math::Point<3>::toVec(this->origin, screenPoint).normalized()
        );
    }

}
