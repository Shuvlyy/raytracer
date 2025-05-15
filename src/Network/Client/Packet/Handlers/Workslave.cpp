#include "Workslave.hpp"
#include "Network/Packet/Packets/Workslave.hpp"
#include "Network/Packet/Packets/Finito.hpp"

#include <thread>

#include "logger/Logger.hpp"
#include "Multithreading/Multithreading.hpp"

namespace raytracer::network::packet::client::handler
{

    static void
    renderAndSendResult
    (
        app::Client &cli,
        const packet::Workslave &p
    )
    {
        renderer::Tile tile(p.getX(), p.getY(), p.getWidth(), p.getHeight());

        multithreading::render(
            cli.getRenderer(),
            cli.nbThread(),
            std::ref(tile),
            std::ref(cli.shouldStop())
        );
        if (cli.shouldStop().load() != true)
            cli.push(std::make_unique<Finito>(cli.getRenderer().getRender()->getData()));
    }

    void
    Workslave::handle
    (
        const Packet &packet,
        app::Client &cli
    )
        const
    {
        LOG_DEBUG("Called.");

        const auto &p = reinterpret_cast<const packet::Workslave &>(packet);
        cli.setRenderer(yml::Yml(p.getSceneContent()));
        std::thread(&renderAndSendResult, std::ref(cli), p);
    }

}
