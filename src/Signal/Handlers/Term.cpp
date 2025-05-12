#include "Term.hpp"
#include "App.hpp"
#include "Logger.hpp"

namespace raytracer::signal::handler
{

    void
    Term::handle
    (
        App *app
    )
        const
    {
        LOG_DEBUG("SIGINT TRIGGERED");
        // app->stop();
    }

}
