#pragma once

#define TEXT_FONT   "assets/fonts/Roboto.ttf"

#include "App/App.hpp"
#include "Network/Server/Server.hpp"
#include "Parser/Parser.hpp"

#include <SFML/Graphics.hpp>

namespace raytracer::app
{

    class Server;

    namespace server
    {

        struct Ui
        {
            sf::Text _status;
            sf::Font _font = sf::Font();

            Ui()
            {
                this->_font.loadFromFile(TEXT_FONT);
                this->_status.setFont(_font);
                this->_status.setCharacterSize(32);
                this->_status.setFillColor(sf::Color::White);
                this->_status.setPosition(10, 10);
            }

            void draw(sf::RenderTarget& target) { target.draw(this->_status); }
        };

    }

    class Server final
        : public App
    {
    public:
        explicit Server(const Attributes& attributes);

        void run() override;
        void stop() override;

    private:
        network::Server _server;
        std::unique_ptr<sf::RenderWindow> _previewWindow;
        Preview _preview;
        bool _hasLoadedPreview;
        size_t _currentScene;
        server::Ui _ui;

        void runWindow();

        void updatePreview(
            const std::unique_ptr<Image>& img,
            uint32_t width,
            uint32_t height
        );

        void updateUi();
    };

}
