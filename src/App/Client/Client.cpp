#include "Client.hpp"

namespace raytracer::app
{

    Client::Client
    (
        const Attributes& attributes
    )
        : App(attributes)
    {}

    void
    Client::run()
    {
        LOG_INFO("Client running...");
    }

    void
    Client::stop()
    {
        LOG_INFO("Client stopping...");
    }

}
