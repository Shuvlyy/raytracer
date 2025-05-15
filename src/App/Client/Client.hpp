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

        void push(std::unique_ptr<network::Packet> packet) { this->_client.pushPacket(std::move(packet)); }

        Renderer &getRenderer() { return this->_renderer; }

        uint16_t nbThread() const { return this->_attributes.threadsAmount; }

        void stopRender() { this->_shouldStop = true; }
        std::atomic<bool> &shouldStop() { return this->_shouldStop; }

    private:
        network::Client _client;
        network::packet::client::Manager _manager;
        std::atomic<bool> _running{false};
        std::atomic<bool> _shouldStop{false};
        Renderer _renderer;
        // ...
    };

}
