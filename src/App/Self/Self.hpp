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
        Preview _preview;
        std::unique_ptr<sf::RenderWindow> _window;
        std::atomic<bool> _shouldStop;

        void runWindow();
        void updatePreview(
            const std::unique_ptr<Image>& img,
            uint32_t width,
            uint32_t height
        );
    };

}
