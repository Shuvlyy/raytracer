#include "Workslave.hpp"
#include "Network/Packet/Packets/Workslave.hpp"
#include "logger/Logger.hpp"

namespace raytracer::network::packet::client::handler
{

    void
    Workslave::handle
    (
        const Packet &packet,
        app::Client &cli
    )
        const
    {
        LOG_DEBUG("[packet::handler::WORKSLAVE] Called.");

        const auto &p = reinterpret_cast<const packet::Workslave &>(packet);

        cli.setRenderer(yml::Yml(p.getSceneContent()));
        cli.render(p.getX(), p.getY(), p.getHeight(), p.getWidth());
    }

}
