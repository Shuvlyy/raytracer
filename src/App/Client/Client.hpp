#pragma once

#include <atomic>
#include <future>

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

        void setException(const std::exception_ptr &e) {
            try {
                this->_exceptionPromise.set_exception(e);
            } catch (const std::future_error &fe) {
                LOG_FATAL("Idk wtf is happening: " + std::string(fe.what()));
            }
        }

        [[nodiscard]] std::atomic<size_t> getTotal() const { return this->_totalPixel.load(); }

        void setTotal(const size_t tile) { this->_totalPixel.store(tile); }

    private:
        network::Client _client;
        network::packet::client::Manager _manager;
        std::promise<std::exception_ptr> _exceptionPromise;
        std::atomic<bool> _running{false};
        std::atomic<bool> _shouldStop{false};
        std::atomic<size_t> _totalPixel{0};
        Renderer _renderer;
    };

}
