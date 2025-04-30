#pragma once

namespace raytracer
{

    /**
     * @brief   Abstract base class representing a light source in a scene.
     *
     * This Interface allows sampling light contribution at a specific point in
     * the scene.
     */
    class Light
    {
    public:
        virtual ~Light() = default;

        /**
         * @brief   Samples the light's contribution at a given point.
         *
         * Computes the incoming light direction, distance to the light, and
         * the color intensity from the light source at the given point.
         *
         * @param   point               The point in the scene to sample light
         *                              for
         * @param   directionToLight    Output parameter for the normalized
         *                              direction vector pointing toward the
         *                              light
         * @param   distance            Output parameter for the distance from
         *                              the point to the light
         * @returns The color/intensity of the light at the specified point
         */
        [[nodiscard]] virtual math::Color sample(
            const math::Vec<3>& point,
            math::Vec<3>& directionToLight,
            double& distance
        ) const = 0;

        /**
         * @brief   Indicates whether the light is ambient (non-directional and
         *          infinite).
         *
         * @return  True if the light is ambient, false otherwise
         */
        [[nodiscard]] virtual bool isAmbient() const { return false; }
    };

}
