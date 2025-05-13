#include "Int.hpp"
#include "App/App.hpp"

namespace raytracer::signal::handler
{

    void
    Int::handle
    (
        App *app
    )
        const
    {
        app->stop();
    }

}
