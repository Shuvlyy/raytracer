#pragma once

#include "Light/Light.hpp"

namespace raytracer::light
{

    /**
     * Directional lights represent sources like the sun, where all light rays
     * are parallel, and the light has no specific position. Intensity does not
     * diminish with distance.
     */
    class Directional final
        : public Light
    {
    public:
        /**
         * @brief   Constructs a directional light.
         *
         * @param   color       The color/intensity of the light
         * @param   direction   The direction from which the light is coming
         *                      (will be negated in shading)
         */
        Directional
        (
            const math::Color& color,
            const math::Vec<3>& direction
        ) : _color(color), _direction(direction) {}

        [[nodiscard]] math::Color sample
        (
            [[maybe_unused]] const math::Vec<3>&,
            math::Vec<3>& directionToNormal,
            double& distance
        )
            const override
        {
            directionToNormal = -this->_direction;
            distance = std::numeric_limits<double>::infinity();
            return this->_color;
        }

    private:
        math::Color _color;
        math::Vec<3> _direction;
    };

}
