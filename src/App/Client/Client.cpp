#include "Exception/Exceptions/EmptyPacket.hpp"
#include "Exception/Exceptions/ServerDisconnected.hpp"
#include "Exception/Exceptions/StandardFunctionFail.hpp"

#include "Client.hpp"

#include <poll.h>

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
        this->_running = true;
        LOG_INFO("Client running...");
        this->_client.run();
    }

    void
    Client::stop()
    {
        this->_running = false;
        LOG_INFO("Client stopping...");
    }

}
