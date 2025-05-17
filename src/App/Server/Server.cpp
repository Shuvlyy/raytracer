#include "Server.hpp"

#include <thread>

namespace raytracer::app
{

    Server::Server
    (
        const Attributes& attributes
    )
        : App(attributes),
          _server({
              attributes.port,
              attributes.serverConfigFilepath,
              attributes.sceneFilepaths
          }),
          _previewWindow(
              sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP),
              WIN_TITLE " server",
              sf::Style::Close,
              sf::ContextSettings(0, 0, 8)
          ),
          _hasLoadedPreview(false),
          _currentScene(0)
    {}

    void
    Server::run()
    {
        std::thread serverThread(
            &network::Server::start,
            std::ref(this->_server)
        );

        this->runWindow();

        serverThread.join();
    }

    void
    Server::stop()
    {
        this->_server.stop();

        this->_previewWindow.close();
    }

    void
    Server::runWindow()
    {
        const auto& img = this->_server.getCluster().getResult();

        sf::Event event{};

        while (this->_previewWindow.isOpen()) {
            while (this->_previewWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    this->stop();
                    return;
                }
            }

            if (img == nullptr) {
                continue;
            }

            if (this->_currentScene != this->_server.getCurrentlyProcessingScene()) {
                LOG_INFO("Scene changed, updating preview...");
                this->_currentScene = this->_server.getCurrentlyProcessingScene();
                this->_hasLoadedPreview = false;
                continue;
            }

            const auto& dim = img->getDimensions();

            if (!this->_hasLoadedPreview) {
                this->_preview._previewImage.create(dim[0], dim[1], sf::Color::Black);
                this->_preview._previewTexture.loadFromImage(this->_preview._previewImage);
                this->_preview._previewSprite.setTexture(this->_preview._previewTexture);
                this->_hasLoadedPreview = true;
            }

            try {
                for (uint32_t y = 0; y < dim[1]; y++) {
                    for (uint32_t x = 0; x < dim[0]; x++) {
                        math::Color pixel = img->at(x, y);
                        sf::Color color = {
                            static_cast<sf::Uint8>(pixel[0] * 255),
                            static_cast<sf::Uint8>(pixel[1] * 255),
                            static_cast<sf::Uint8>(pixel[2] * 255)
                        };

                        this->_preview._previewImage.setPixel(x, y, color);
                    }
                }

                this->_preview._previewTexture.update(this->_preview._previewImage);
            }
            catch (...) {
                // ...
            }

            this->_previewWindow.clear();
            this->_previewWindow.draw(this->_preview._previewSprite);
            this->_previewWindow.display();
        }
    }

}
