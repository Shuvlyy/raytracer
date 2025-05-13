#include "Server.hpp"

namespace raytracer::app
{

    Server::Server
    (
        const Attributes& attributes
    )
        : App(attributes),
          _server({
              attributes.port,
              attributes.serverConfigFilepath,
              attributes.sceneFilepath
          })
    {}

    void
    Server::run()
    {
        this->_server.start();
    }

    void
    Server::stop()
    {
        this->_server.stop();
    }

}
