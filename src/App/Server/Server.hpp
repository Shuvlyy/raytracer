#pragma once

#include "App/App.hpp"
#include "Network/Server/Server.hpp"
#include "Parser/Parser.hpp"

#include <SFML/Graphics.hpp>

namespace raytracer::app
{

    namespace server
    {

        struct Preview
        {
            sf::Image _previewImage;
            sf::Sprite _previewSprite;
            sf::Texture _previewTexture;
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
        sf::RenderWindow _previewWindow;
        server::Preview _preview;
        bool _hasLoadedPreview;

        void runWindow();
    };

}
