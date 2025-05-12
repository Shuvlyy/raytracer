#pragma once

#define WIN_WIDTH   1920
#define WIN_HEIGHT  1080
#define WIN_BPP     32
#define WIN_FPS     60

#include "Renderer.hpp"
#include "Network/Server/Server.hpp"
#include "Signal/Manager.hpp"

#include <SFML/Graphics.hpp>

namespace raytracer
{

    class App final
    {
    public:
        explicit App(const yml::Yml& yml);

        void run();

    private:
        const yml::Yml& _config;
        Renderer _renderer;
        sf::RenderWindow _window;
        std::unique_ptr<Server> _server;
        signal::Manager _signalManager;
    };

}
