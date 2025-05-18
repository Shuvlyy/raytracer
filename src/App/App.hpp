#pragma once

#define WIN_WIDTH   1920
#define WIN_HEIGHT  1080
#define WIN_BPP     32
#define WIN_FPS     30
#define WIN_TITLE   "Raytracer"

#include "Parser/Parser.hpp"
#include "Signal/Manager.hpp"
#include "SFML/Graphics.hpp"

namespace raytracer
{

    namespace app
    {

        struct Preview
        {
            sf::Image _previewImage;
            sf::Sprite _previewSprite;
            sf::Texture _previewTexture;
        };

    }

    class App
    {
    public:
        explicit App(const Attributes& attributes)
            : _attributes(attributes), _signalManager(this) {}

        virtual ~App() = default;

        virtual void run() = 0;
        virtual void stop() = 0;

    protected:
        const Attributes& _attributes;

    private:
        signal::Manager _signalManager;
    };

}
