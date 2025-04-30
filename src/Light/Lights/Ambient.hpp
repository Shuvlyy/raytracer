#pragma once

#include "Math/Color.hpp"
#include "Light/Light.hpp"

namespace raytracer::light
{

    class Ambient final
        : public Light
    {
    public:
        /**
         * @brief   Constructs an Ambient light with a specified color and
         *          intensity.
         *
         * @param   color       The color of the ambient light
         * @param   intensity   The brightness of the ambient light
         */
        explicit Ambient(
            const math::Color& color,
            double intensity = 1.0
        ) : _color(color), _intensity(intensity) {}

        [[nodiscard]] math::Color sample
        (
            const math::Vec<3> &,   //
            math::Vec<3> &,         // No usage
            double &                //
        )
            const override
        {
            return this->_color * this->_intensity;
        }

        [[nodiscard]] bool isAmbient() const override { return true; }

    private:
        math::Color _color;
        double _intensity;
    };

}
