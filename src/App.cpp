#include "App.hpp"
#include "Multithreading/Multithreading.hpp"
#include "Logger.hpp"

#include <filesystem>

namespace raytracer
{

    App::App
    (
        const yml::Yml& yml
    )
        : _config(yml), _renderer(yml),
          _window(
              sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP),
              "Raytracer",
              sf::Style::Close,
              sf::ContextSettings(0, 0, 8)
          ),
          _signalManager(this)
    {
        this->_window.setFramerateLimit(WIN_FPS);
        // TODO: If server asked, then initialize server and everything.
    }

    void App::run()
    {
        namespace fs = std::filesystem;

        multithreading::render(this->_renderer);

        // TODO: Show render in the window in real-time (another thread)

        const std::unique_ptr<Image>& image = this->_renderer.getRender();

        const auto outputDirectory =
            this->_config["outputDirectory"].as<std::string>();

        if (!fs::exists(outputDirectory)) {
            fs::create_directory(outputDirectory);
        }

        image->save(outputDirectory + "/" + Logger::getFormattedCurrentTimestamp());
    }

}
