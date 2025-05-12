#pragma once

#include "App/App.hpp"
#include "Network/Server/Server.hpp"
#include "Parser/Parser.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace raytracer::app
{

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
    };

}
