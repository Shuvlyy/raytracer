#include "Multithreading.hpp"
#include "Logger.hpp"

#include <filesystem>
#include <format>
#include <fstream>
#include <thread>

static int
getNbProcs
()
{
    if (!std::filesystem::exists(CPU_INFO_FILE))
        return 1;
    std::ifstream file(CPU_INFO_FILE);
    if (!file.is_open()) {
        LOG_WARN("Couldn't open cpu file info, disabling multithreading");
        return 1;
    }
    std::filebuf *s = file.rdbuf();
    std::stringstream buffer;
    buffer << s;
    std::string temp = buffer.str();
    file.close();
    return std::stoi(temp.substr(2)) + 1;
}

namespace raytracer::multithreading {

    void
    render
    (
        Renderer &renderer
    )
    {
        const int nbProcs = getNbProcs();
        int last_y = 0;
        int portion_size = std::floor(renderer.getHeight() / nbProcs);
        LOG_INFO("Starting render with " + std::to_string(nbProcs) + " threads.");
        std::vector<std::thread> threads;

        if (nbProcs == 1) {
            renderer.render(0, renderer.getHeight());
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
        for (auto & thread : threads) {
            thread.join();
        }
        LOG_INFO("Render finished.");
    }

}
