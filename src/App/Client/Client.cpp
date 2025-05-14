#include "Network/Client/Packet/IHandler.hpp"

#include "Client.hpp"

#include <poll.h>
#include <thread>

namespace raytracer::app
{

    Client::Client
    (
        const Attributes& attributes
    )
        : App(attributes),
          _client(attributes.host, attributes.port)
    {}

    void
    Client::run()
    {
        LOG_INFO("Client running...");
        this->_running = true;
        std::thread th(&network::Client::run, &this->_client);
        while (this->_running) {
            if (this->_client.hasPacketToProcess()) {
                auto packet = this->_client.popPacket();
                this->_manager.dispatchPacket(*packet, *this);
            }
        }
    }

    void
    Client::stop()
    {
        LOG_INFO("Client stopping...");
        this->_running = false;
        this->_client.stop();
    }

}
