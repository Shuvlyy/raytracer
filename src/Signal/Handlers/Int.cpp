#include "Int.hpp"
#include "App.hpp"
#include "Logger.hpp"

namespace raytracer::signal::handler
{

    void
    Int::handle
    (
        App *app
    )
        const
    {
        LOG_DEBUG("SIGINT TRIGGERED");
        // app->stop();
    }

}

