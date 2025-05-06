#pragma once

#include "Light/Light.hpp"

#define POINTS "points"

namespace raytracer::light
{

    /**
     * A point is a light source that emits light equally in all directions
     * from a single position.
     *
     * The light intensity follows the inverse-square law, meaning its
     * contribution diminishes with the square of the distance from the source.
     * This class models realistic falloff behavior, including a small epsilon
     * to prevent division by zero (infinite bounces).
     */
    class Point final
        : public Light
    {
    public:
        /**
         * @brief   Constructs a point light.
         *
         * @param   color       The base color of the light
         * @param   position    The position of the light
         * @param   intensity   The light’s radiant intensity (scaling factor
         *                      for brightness)
         */
        explicit Point
        (
            const math::Color& color,
            const math::Vec<3>& position,
            const double intensity = 1.0
        ) : _color(color), _position(position), _intensity(intensity) {}

        [[nodiscard]] math::Color sample
        (
            const math::Vec<3>& point,
            math::Vec<3>& directionToNormal,
            double& distance
        )
            const override
        {
            const math::Vec<3> delta = this->_position - point;

            distance = delta.length();
            directionToNormal = delta / distance;
            return (this->_intensity * this->_color) / (4 * std::numbers::pi * std::max(distance * distance, .01));
        }

    private:
        math::Color _color;
        math::Vec<3> _position;
        double _intensity;
    };

}
