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
          _config(attributes.sceneFilepaths.at(0)), // TODO: Finish this lol
          _renderer(this->_config),
          _shouldStop(false)
    {
        if (!this->_attributes.noPreview) {
            this->_window = std::make_unique<sf::RenderWindow>(
                sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP),
                WIN_TITLE,
                sf::Style::Close,
                sf::ContextSettings(0, 0, 8)
            );
            this->_window->setFramerateLimit(WIN_FPS);
        }
    }

    void
    Self::run()
    {
        namespace fs = std::filesystem;

        uint32_t tileWidth = this->_attributes.tileSize == -1 ? DEFAULT_SELF_TILE_SIZE : this->_attributes.tileSize;
        uint32_t tileHeight = tileWidth;

        renderer::Tile tile(0, 0, this->_renderer.getWidth(), this->_renderer.getHeight() - 1);

        std::thread renderThread(
            &multithreading::render,
            std::ref(this->_renderer),
            this->_attributes.threadsAmount,
            std::ref(tile),
            std::ref(this->_shouldStop),
            tileWidth, tileHeight
        );

        if (!this->_attributes.noPreview) {
            this->runWindow();
        }

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

        if (!this->_attributes.noPreview && this->_window->isOpen()) {
            this->_window->close();
        }
    }

    // FIXME: Not very optimised but 😞
    void
    Self::runWindow()
    {
        const auto& img = this->_renderer.getRender();
        const auto& dim = img->getDimensions();

        sf::Event event{};

        this->_preview._previewImage.create(dim[0], dim[1]);
        this->_preview._previewTexture.loadFromImage(this->_preview._previewImage);
        this->_preview._previewSprite.setTexture(this->_preview._previewTexture, true);

        const auto winSize = this->_window->getSize();
        const float scaleX = static_cast<float>(winSize.x) / static_cast<float>(dim[0]);
        const float scaleY = static_cast<float>(winSize.y) / static_cast<float>(dim[1]);
        const float uniformScale = std::min(scaleX, scaleY);

        this->_preview._previewSprite.setScale(uniformScale, uniformScale);

        while (!this->_shouldStop.load()) {
            while (this->_window->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    this->stop();
                    return;
                }
            }

            this->updatePreview(img, dim[0], dim[1]);

            this->_preview._previewTexture.update(this->_preview._previewImage);

            this->_window->clear();
            this->_window->draw(this->_preview._previewSprite);
            this->_window->display();
        }
    }

    void
    Self::updatePreview
    (
        const std::unique_ptr<Image>& img,
        const uint32_t width,
        const uint32_t height
    )
    {
        if (width > this->_preview._previewImage.getSize().x ||
            height > this->_preview._previewImage.getSize().y) {
            return;
        }

        for (uint32_t y = 0; y < height; y++) {
            for (uint32_t x = 0; x < width; x++) {
                math::Color pixel = img->at(x, y);
                sf::Color color = {
                    static_cast<sf::Uint8>(pixel[0] * 255),
                    static_cast<sf::Uint8>(pixel[1] * 255),
                    static_cast<sf::Uint8>(pixel[2] * 255)
                };

                this->_preview._previewImage.setPixel(x, y, color);
            }
        }
    }

}
