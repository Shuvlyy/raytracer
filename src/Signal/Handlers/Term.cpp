#include "Term.hpp"
#include "App/App.hpp"

namespace raytracer::signal::handler
{

    void
    Term::handle
    (
        App *app
    )
        const
    {
        app->stop();
    }

}
