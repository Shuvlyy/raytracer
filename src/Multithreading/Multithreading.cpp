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
        const int tileWidth,
        const int tileHeight,
        const std::atomic<bool>& shouldStop
    )
    {
        LOG_INFO("Starting render with " + std::to_string(nbProcs) + " threads.");

        if (nbProcs == 1) {
            renderer.render(
                0, 0,
                renderer.getWidth(), renderer.getHeight() - 1,
                shouldStop
            );
            LOG_INFO("Render finished.");
            return;
        }

        const int width = static_cast<int>(renderer.getWidth());
        const int height = static_cast<int>(renderer.getHeight() - 1);

        const int tilesX = (width + tileWidth - 1) / tileWidth;
        const int tilesY = (height + tileHeight - 1) / tileHeight;

        std::vector<renderer::Tile> tiles;

        for (int y = 0; y < tilesY; y++) {
            for (int x = 0; x < tilesX; x++) {
                tiles.emplace_back(
                    x * tileWidth,
                    y * tileHeight,
                    std::min(tileWidth, width - x * tileWidth),
                    std::min(tileHeight, height - y * tileHeight)
                );
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
