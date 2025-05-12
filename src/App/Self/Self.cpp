#include "Self.hpp"

#include "Multithreading/Multithreading.hpp"

namespace raytracer::app
{

    Self::Self
    (
        const Attributes& attributes
    )
        : App(attributes),
          _config(attributes.sceneFilepath),
          _renderer(this->_config),
          _window(
              sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP),
              WIN_TITLE,
              sf::Style::Close,
              sf::ContextSettings(0, 0, 8)
          )
    {
        this->_window.setFramerateLimit(WIN_FPS);
    }

    void
    Self::run()
    {
        namespace fs = std::filesystem;

        multithreading::render(this->_renderer, this->_attributes.threadsAmount);

        const std::unique_ptr<Image>& image = this->_renderer.getRender();

        const auto outputDirectory =
            this->_config["outputDirectory"].as<std::string>();

        if (!fs::exists(outputDirectory)) {
            fs::create_directory(outputDirectory);
        }
        image->save(outputDirectory + "/" + Logger::getFormattedCurrentTimestamp());
    }

    void
    Self::stop()
    {
        LOG_INFO("Stopping...");
        // ...
    }

}
