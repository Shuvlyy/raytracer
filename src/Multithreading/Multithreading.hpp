#pragma once

#include "Renderer/Renderer.hpp"

namespace raytracer::multithreading
{

    void render(
        Renderer &renderer,
        int nbProcs,
        int tileWidth,
        int tileHeight,
        const std::atomic<bool>& shouldStop
    );

}
