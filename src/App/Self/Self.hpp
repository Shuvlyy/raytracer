#pragma once

#include "App/App.hpp"
#include "Renderer/Renderer.hpp"
#include "Parser/Parser.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace raytracer::app
{

    class Self final
        : public App
    {
    public:
        explicit Self(const Attributes& attributes);

        void run() override;
        void stop() override;

    private:
        yml::Yml _config;
        Renderer _renderer;
        sf::RenderWindow _window;
        std::atomic<bool> _shouldStop;

        void runWindow();
    };

}
