#pragma once

#include "Renderer/Renderer.hpp"
#include "Renderer/Tile.hpp"

namespace raytracer::multithreading
{

    void render(
        Renderer &renderer,
        int nbProcs,
        const renderer::Tile &tileToRender,
        const std::atomic<bool>& shouldStop
    );
}
