#include "Server.hpp"

namespace raytracer::app
{

    Server::Server
    (
        const Attributes& attributes
    )
        : App(attributes),
          _server({ attributes.port })
    {}

    void
    Server::run()
    {
        LOG_INFO("Server running...");
    }

    void
    Server::stop()
    {
        LOG_INFO("Server stopping...");
    }

}
