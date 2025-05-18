#include "Network/Client/Packet/IHandler.hpp"

#include "Client.hpp"

#include <poll.h>
#include <thread>

#include "Exception/Exceptions/ClientDisconnected.hpp"

namespace raytracer::app
{

    Client::Client
    (
        const Attributes& attributes
    )
        : App(attributes),
          _client(attributes.host, attributes.port),
          _tileWidth(attributes.tileSize == -1 ? DEFAULT_CLIENT_TILE_SIZE : attributes.tileSize),
          _tileHeight(this->_tileWidth)
    {}

    void
    Client::run()
    {
        LOG_INFO("Client running...");
        this->_running = true;
        auto exFuture = this->_exceptionPromise.get_future();

        std::thread (&network::Client::run, &this->_client, std::ref(*this)).detach();
        while (this->_running.load()) {
            if (exFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                try {
                    if (std::exception_ptr eptr = exFuture.get()) {
                        std::rethrow_exception(eptr);
                    }
                } catch (const exception::ClientDisconnected &) {
                    LOG_FATAL("Server disconnected");
                }
                this->stop();
                return;
            }
            if (this->_client.hasPacketToProcess()) {
                auto packet = this->_client.popPacket();
                if (packet != nullptr) {
                    this->_manager.dispatchPacket(*packet, *this);
                }
            }
        }
    }

    void
    Client::stop()
    {
        this->_shouldStop = true;
        this->_running = false;
        this->_client.stop();
        LOG_INFO("Client stopped");
    }

}
