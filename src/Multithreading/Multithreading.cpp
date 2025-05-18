#include "Multithreading.hpp"
#include "logger/Logger.hpp"
#include "Common/Utils.hpp"

#include <format>
#include <fstream>
#include <thread>

namespace raytracer::multithreading
{

    void
    render
    (
        Renderer &renderer,
        const int nbProcs,
        const renderer::Tile &tileToRender,
        const std::atomic<bool>& shouldStop,
        const uint32_t tileWidth, const uint32_t tileHeight
    )
    {
        LOG_INFO("Starting render with " + std::to_string(nbProcs) + " threads.");

        if (nbProcs == 1) {
            renderer.render(
                0, 0,
                tileToRender.width, tileToRender.height - 1,
                shouldStop
            );
            LOG_INFO("Render finished.");
            return;
        }

        const uint32_t tilesX = (tileToRender.width + tileWidth - 1) / tileWidth;
        const uint32_t tilesY = (tileToRender.height + tileHeight - 1) / tileHeight;

        std::vector<renderer::Tile> tiles;

        for (uint32_t y = 0; y < tilesY; y++) {
            for (uint32_t x = 0; x < tilesX; x++) {
                const uint32_t tileStartX = tileToRender.x + x * tileWidth;
                const uint32_t tileStartY = tileToRender.y + y * tileHeight;

                const uint32_t w = std::min(tileWidth, tileToRender.x + tileToRender.width - tileStartX);
                const uint32_t h = std::min(tileHeight, tileToRender.y + tileToRender.height - tileStartY);

                if (w > 0 && h > 0) {
                    tiles.emplace_back(tileStartX, tileStartY, w, h);
                }
            }
        }

        std::atomic<size_t> nextTile = 0;
        std::vector<std::thread> threads;

        renderer.setTiles(std::move(tiles));
        auto& tileList = renderer.getTiles();

        for (int k = 0; k < nbProcs; ++k) {
            threads.emplace_back([&, k]() {
                while (shouldStop.load() == false) {
                    const size_t index = nextTile.fetch_add(1);

                    if (index >= tileList.size() || shouldStop.load() == true) {
                        break;
                    }

                    renderer::Tile& tile = tileList.at(index);
                    tile.threadNumber = k;

                    renderer.render(tile, shouldStop);

                    tile.threadNumber = -1; // done
                }
            });
        }

        for (auto& thread: threads) {
            thread.join();
        }

        LOG_INFO("Render finished.");
    }

}
