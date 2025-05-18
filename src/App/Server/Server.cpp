#include "Server.hpp"

#include <thread>

#include "Camera/Camera.hpp"
#include "Exception/IException.hpp"

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
              attributes.sceneFilepaths,
              attributes.tileSize
          }),
          _hasLoadedPreview(false),
          _currentScene(0)
    {
        if (!this->_attributes.noPreview) {
            this->_previewWindow = std::make_unique<sf::RenderWindow>(
                sf::VideoMode(WIN_WIDTH, WIN_HEIGHT, WIN_BPP),
                WIN_TITLE " server",
                sf::Style::Close,
                sf::ContextSettings(0, 0, 8)
            );
            this->_previewWindow->setFramerateLimit(WIN_FPS);
        }
    }

    void
    Server::run()
    {
        std::thread serverThread(
            &network::Server::start,
            std::ref(this->_server)
        );

        if (!this->_attributes.noPreview) {
            this->runWindow();
        }

        serverThread.join();
    }

    void
    Server::stop() {
        this->_server.stop();

        if (!this->_attributes.noPreview) {
            this->_previewWindow->close();
        }
    }

    void
    Server::runWindow()
    {
        const auto& img = this->_server.getCluster().getResult();

        sf::Event event{};

        while (this->_previewWindow->isOpen()) {
            while (this->_previewWindow->pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    this->stop();
                    return;
                }
            }

            if (this->_currentScene == this->_attributes.sceneFilepaths.size()) {
                continue; // No need to recalculate the image.
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
                LOG_INFO("Updating preview with {" + std::to_string(dim[0]) + "," + std::to_string(dim[1]) + "}");
                LOG_INFO("Current scene: " + std::to_string(this->_currentScene));
                LOG_INFO("Path: " + this->_attributes.sceneFilepaths.at(this->_currentScene));
                yml::Yml yml(this->_attributes.sceneFilepaths.at(this->_currentScene));
                Camera cam = Camera::fromConfig(yml);

                const uint32_t width = cam.width;
                const uint32_t height = cam.height;

                this->_previewWindow->clear();
                this->_preview._previewImage.create(width, height);
                this->_preview._previewTexture.loadFromImage(this->_preview._previewImage);
                this->_preview._previewSprite.setTexture(this->_preview._previewTexture, true);

                const auto winSize = this->_previewWindow->getSize();
                const float scaleX = static_cast<float>(winSize.x) / static_cast<float>(width);
                const float scaleY = static_cast<float>(winSize.y) / static_cast<float>(height);
                const float uniformScale = std::min(scaleX, scaleY);

                this->_preview._previewSprite.setScale(uniformScale, uniformScale);

                this->_hasLoadedPreview = true;
            }

            this->updatePreview(img, dim[0], dim[1]);
            this->updateUi();

            this->_previewWindow->clear();
            this->_previewWindow->draw(this->_preview._previewSprite);
            this->_ui.draw(*this->_previewWindow);
            this->_previewWindow->display();
        }
    }

    void
    Server::updatePreview
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

        try {
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

            this->_preview._previewTexture.update(this->_preview._previewImage);
        }
        catch (const exception::IException&) {
            // LALALA J4ENTEND PAS
        }
    }

    void
    Server::updateUi()
    {
        network::Server& server = this->_server;
        network::server::Cluster& cluster = server.getCluster();
        const std::vector<std::string>& sceneFilepaths = this->_attributes.sceneFilepaths;

        double progress = 0;
        if (cluster.getTotalTilesN() > 0) {
            progress = static_cast<double>(cluster.getDoneTilesN()) / static_cast<double>(cluster.getTotalTilesN()) * 100;
        }

        std::string text = std::format(
            "Cluster status: {}\n"
            "Current scene: {} ({}/{})\n"
            "Clients connected: {}/{}\n"
            "Total tiles: {}\n"
            "Tiles left: {}\n"
            "Tiles in progress: {}\n"
            "Tiles done: {}\n"
            "[{:.2f}%]",
            network::server::cluster::stateToString(cluster.getState()),
            sceneFilepaths.at(this->_currentScene), this->_currentScene + 1, sceneFilepaths.size(),
            server.getSessionManager().getSessions().size(), server.getSettings().maxClients,
            cluster.getTotalTilesN(),
            cluster.getPendingTilesN(),
            cluster.getAssignedTilesN(),
            cluster.getDoneTilesN(),
            progress
        );

        this->_ui._status.setString(text);
    }

}
