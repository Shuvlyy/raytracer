#include "Renderer.hpp"

namespace raytracer
{

    Renderer::Renderer
    (
        const yml::Yml &yml
    )
        : _camera(Camera::fromConfig(yml)),
          _scene(yml)
    {
        yml::Node resolutionNode = yml["camera"]["resolution"];

        this->_width = resolutionNode["width"].as<int>();
        this->_height = resolutionNode["height"].as<int>();
    }

    image::Ppm
    Renderer::render()
        const
    {
        image::Ppm image(this->_width, this->_height); // TODO: Change type of image according to user's choice (in a long time lol)

        for (uint32_t y = 0; y < this->_height; y++) {
            for (uint32_t x = 0; x < this->_width; x++) {
                double u = double(x) / this->_width;
                double v = double(y) / this->_height;

                const math::Ray ray = this->_camera.ray(u, v);

                // ...
            }
        }
        return image;
    }

}
