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
        int nbProcs
    )
    {
        int last_y = 0;
        int portion_size = std::floor(renderer.getHeight() / nbProcs);
        LOG_INFO("Starting render with " + std::to_string(nbProcs) + " threads.");
        std::vector<std::thread> threads;

        if (nbProcs == 1) {
            renderer.render(0, renderer.getHeight() - 1);
            return;
        }
        for (int i = 1; i < nbProcs + 1; i++) {
            if (i == nbProcs) {
                threads.emplace_back(&Renderer::render, &renderer, last_y, renderer.getHeight() - 1);
                LOG_INFO(std::format("thread n°{} rendering from {} to {}.", i, last_y, renderer.getHeight() - 1));
            } else {
                threads.emplace_back(&Renderer::render, &renderer, last_y, portion_size * i);
                LOG_INFO(std::format("thread n°{} rendering from {} to {}.", i, last_y, portion_size * i));
            }
            last_y = portion_size * i + 1;
        }
        // TODO: SFML display
        for (auto & thread : threads) {
            thread.join();
        }
        LOG_INFO("Render finished.");
    }

}
