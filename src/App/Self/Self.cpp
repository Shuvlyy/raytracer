#include "Self.hpp"

#include "Multithreading/Multithreading.hpp"

#include <thread>
#include <SFML/Graphics.hpp>

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
          ),
          _shouldStop(false)
    {
        this->_window.setFramerateLimit(WIN_FPS);
    }

    void
    Self::run()
    {
        namespace fs = std::filesystem;

        std::thread renderThread(
            &multithreading::render,
            std::ref(this->_renderer),
            this->_attributes.threadsAmount,
            64, 64,
            std::ref(this->_shouldStop)
        );

        this->runWindow();

        renderThread.join();

        const auto& imageOutput = this->_renderer.getRender();

        const auto outputDirectory =
            this->_config["outputDirectory"].as<std::string>();

        if (!fs::exists(outputDirectory)) {
            fs::create_directory(outputDirectory);
        }
        imageOutput->save(outputDirectory + "/" + Logger::getFormattedCurrentTimestamp());
    }

    void
    Self::stop()
    {
        LOG_INFO("Stopping...");

        this->_shouldStop = true;

        if (this->_window.isOpen()) {
            this->_window.close();
        }
    }

    // FIXME: Not very optimised but 😞
    void
    Self::runWindow()
    {
        const auto& img = this->_renderer.getRender();
        const auto& dim = img->getDimensions();

        sf::Event event{};
        sf::Image image;
        sf::Sprite sprite;
        sf::Texture texture;

        image.create(dim[0], dim[1]);

        texture.loadFromImage(image);
        sprite.setTexture(texture);

        while (!this->_shouldStop.load()) {
            while (this->_window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    this->stop();
                    return;
                }
            }

            for (uint32_t y = 0; y < dim[1]; y++) {
                for (uint32_t x = 0; x < dim[0]; x++) {
                    math::Color pixel = img->at(x, y);
                    sf::Color color = {
                        static_cast<sf::Uint8>(pixel[0] * 255),
                        static_cast<sf::Uint8>(pixel[1] * 255),
                        static_cast<sf::Uint8>(pixel[2] * 255)
                    };

                    image.setPixel(x, y, color);
                }
            }

            texture.update(image);

            this->_window.clear();
            this->_window.draw(sprite);
            this->_window.display();
        }
    }

}
