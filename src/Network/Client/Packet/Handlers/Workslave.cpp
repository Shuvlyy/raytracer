#include "Workslave.hpp"
#include "Network/Packet/Packets/Workslave.hpp"
#include "Network/Packet/Packets/Finito.hpp"
#include "Exception/IException.hpp"

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
        try {
            renderer::Tile tile(p.getX(), p.getY(), p.getWidth(), p.getHeight() - 1);

            renderer::totalComputedPixels.store(0);
            cli.setTotal(p.getWidth() * p.getHeight());
            multithreading::render(
                cli.getRenderer(),
                cli.nbThread(),
                std::ref(tile),
                std::ref(cli.shouldStop())
            );
            if (cli.shouldStop().load() != true) {
                PixelBuffer& buf = cli.getRenderer().getRender()->getData();
                std::ranges::transform(
                    buf,
                    buf.begin(),
                    [](const math::Color &pixel) {
                        return pixel * MAX_COLOR_SHADES;
                    }
                );
                cli.push(std::make_unique<Finito>(cli.getRenderer().getRender()->getData()));
            }
        }
        catch (exception::IException &exception) {
            LOG_ERR(
                "Error during Rendering: " +
                std::string(exception.what())
            );
            cli.setException(std::current_exception());
        }
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
        cli.setRenderer(yml::Yml(p.getSceneContent(), true));
        std::thread(&renderAndSendResult, std::ref(cli), p).detach();
    }

}
