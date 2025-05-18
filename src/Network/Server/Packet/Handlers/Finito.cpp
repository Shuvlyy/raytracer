#include "Finito.hpp"
#include "Network/Packet/Packets/Finito.hpp"

#include "Image/Ppm/Ppm.hpp"
#include "Network/Server/Server.hpp"
#include "logger/Logger.hpp"

namespace raytracer::network::packet::server::handler
{

    void
    Finito::handle
    (
        const Packet& packet,
        Server& server,
        network::server::Session& session
    )
        const
    {
        const auto& p = reinterpret_cast<const packet::Finito&>(packet);
        PixelBuffer pixelBuffer = p.getPixelBuffer();

        std::ranges::transform(
            pixelBuffer,
            pixelBuffer.begin(),
            [](const math::Color &pixel) {
                return pixel / MAX_COLOR_SHADES;
            }
        );

        *server.getCluster().getResult() += pixelBuffer;

        renderer::Tile& tile = session.getData().currentTile;
        server.getCluster().onTileFinished(session);
        LOG_DEBUG("Render done. Tile: x=" + std::to_string(tile.x) + " y= " + std::to_string(tile.y) + " w=" + std::to_string(tile.width) + " h=" + std::to_string(tile.height));

        LOG_DEBUG("Client (SFD: " + std::to_string(session.getId()) + ") finished rendering.");

        session.setState(network::server::session::State::READY);
    }

}
