#pragma once

#include <atomic>

#include "Renderer/Renderer.hpp"
#include "App/App.hpp"
#include "Network/Client/Client.hpp"
#include "Network/Client/Packet/Manager.hpp"
#include "Parser/Parser.hpp"

namespace raytracer::app
{

    class Client final
        : public App
    {
    public:
        explicit Client(const Attributes& attributes);

        void run() override;
        void stop() override;

        void setRenderer(const yml::Yml &config) { this->_renderer = Renderer::fromConfig(config); }

        void render(const uint32_t x, const uint32_t y, const uint32_t w, const uint32_t h) const
            { this->_renderer.render(x, y, w, h, false); }

    private:
        network::Client _client;
        network::packet::client::Manager _manager;
        std::atomic_bool _running{false};
        Renderer _renderer;
        // ...
    };

}
