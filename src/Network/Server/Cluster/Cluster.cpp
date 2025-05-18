#include "Cluster.hpp"
#include "Image/Ppm/Ppm.hpp"
#include "logger/Logger.hpp"
#include "Network/Packet/Packets/Workslave.hpp"
#include "Network/Server/Server.hpp"

#include <filesystem>

namespace raytracer::network::server
{

    Cluster::Cluster
    (
        Server& server
    )
        : _server(server),
          _state(cluster::State::LOADING),
          _heartbeatFrequency(0)
    {}

    void
    Cluster::update()
    {
        this->updateState();

        if (this->_state == cluster::State::WAITING) {
            return;
        }

        this->updateSlavesData();

        if (this->_state == cluster::State::READY) {
            this->startRender();
        }

        if (this->_state == cluster::State::RENDERING) {
            this->checkRenderStatus();
        }
    }

    void
    Cluster::updateState()
    {
        if (this->_state == cluster::State::WAITING) {
            if (this->_slaves.size() == this->_server.getSettings().maxClients) {
                this->_state = cluster::State::READY;
            }
        }
    }

    void
    Cluster::updateSlavesData()
    {
        using namespace std::chrono;

        for (auto& [_, s] : this->_slaves) {
            Session& slave = s.get();

            auto now = steady_clock::now();

            if (duration_cast<seconds>(now - slave.getLastLatencyRefresh()).count() >= this->_heartbeatFrequency) {
                slave.refreshLatency();
                slave.setLastLatencyRefresh(now);

                LOG_DEBUG("Slave latency: " + std::to_string(slave.getLatency()) + " ms");
            }
        }
    }

    void
    Cluster::addSlave
    (
        Session& session
    )
    {
        if (this->_slaves.contains(session.getId())) {
            return;
        }
        this->_slaves.insert({ session.getId(), session });
    }

    void
    Cluster::removeSlave
    (
        const Session& session
    )
    {
        const auto sessionId = session.getId();

        if (!this->_slaves.contains(sessionId)) {
            return;
        }

        /**
         * If slave was working on something, then reassign the tile to someone
         * else.
         */
        if (this->_assignedTiles.contains(sessionId)) {
            this->_pendingTiles.push(this->_assignedTiles.at(sessionId));
            this->_assignedTiles.erase(sessionId);
        }

        this->_slaves.erase(sessionId);
    }

    void Cluster::setupImageOutput
    (
        const yml::Node &dimensionsNode
    )
    {
        const uint32_t width = dimensionsNode["width"].as<int>();
        const uint32_t height = dimensionsNode["height"].as<int>();

        this->setupImageOutput(width, height);
    }

    void
    Cluster::setupImageOutput
    (
        uint32_t width,
        uint32_t height
    )
    {
        this->_result = std::make_unique<image::Ppm>(width, height);
    }

    void
    Cluster::onTileFinished
    (
        Session &session
    )
    {
        const uint32_t id = session.getId();

        if (_assignedTiles.contains(id)) {
            _assignedTiles.erase(id);
        }
    }

    void
    Cluster::startRender()
    {
        this->_state = cluster::State::RENDERING;

        auto dim = this->_result->getDimensions();

        this->createTiles(dim[0], dim[1]);
    }

    void
    Cluster::checkRenderStatus()
    {
        if (this->_pendingTiles.empty() && this->_assignedTiles.empty()) {
            LOG_DEBUG("Everything is finished.");

            namespace fs = std::filesystem;

            const auto outputDirectory =
                this->_server.getSceneConfig()["outputDirectory"].as<std::string>();

            if (!fs::exists(outputDirectory)) {
                fs::create_directory(outputDirectory);
            }
            this->_result->save(outputDirectory + "/" + Logger::getFormattedCurrentTimestamp());

            LOG_INFO("Done.");
            this->_state = cluster::State::FINISHED;

            return;
        }

        LOG_DEBUG("Some to give");

        for (auto& [_, s] : this->_slaves) {
            Session& slave = s.get();

            if (slave.getState() == session::State::READY) {
                LOG_DEBUG("Slave ready for new work, giving...");

                if (this->_pendingTiles.empty()) {
                    LOG_DEBUG("Nothing to give, just not done yet.");
                    return;
                }

                auto& tileToRender = this->_pendingTiles.front();
                this->_pendingTiles.pop();

                slave.setState(session::State::RENDERING);
                this->_assignedTiles[slave.getId()] = tileToRender;
                slave.getData().currentTile = tileToRender;

                packet::Workslave p(
                    this->_server.getSceneConfig().getRawContent(),
                    tileToRender.x, tileToRender.y,
                    tileToRender.width, tileToRender.height
                );

                LOG_DEBUG(
                    "Giving tile\n"
                    "\tx: " + std::to_string(tileToRender.x) + "\n"
                    "\ty: " + std::to_string(tileToRender.y) + "\n"
                    "\tw: " + std::to_string(tileToRender.width) + "\n"
                    "\th: " + std::to_string(tileToRender.height)
                );

                slave.getControlSocket().sendPacket(p.serialize());
            } else if (slave.getState() == session::State::RENDERING) {
                LOG_DEBUG("Slave is still rendering, let it work...");
            }
        }
    }

    void
    Cluster::createTiles
    (
        const uint32_t width,
        const uint32_t height
    )
    {
        this->_pendingTiles = {};
        this->_assignedTiles.clear();
        this->_totalTiles = 0;

        constexpr uint32_t tileWidth = 1024;
        constexpr uint32_t tileHeight = 1024;

        const uint32_t tilesX = (width + tileWidth - 1) / tileWidth;
        const uint32_t tilesY = (height + tileHeight - 1) / tileHeight;

        for (uint32_t y = 0; y < tilesY; y++) {
            for (uint32_t x = 0; x < tilesX; x++) {
                renderer::Tile tile = {
                    x * tileWidth,
                    y * tileHeight,
                    std::min(tileWidth, width - x * tileWidth),
                    std::min(tileHeight, height - y * tileHeight)
                };

                this->_pendingTiles.push(tile);
                this->_totalTiles++;

                LOG_INFO(
                    "Generated tile: x=" + std::to_string(x * tileWidth) +
                    ", y=" + std::to_string(y * tileHeight) +
                    ", w=" + std::to_string(std::min(tileWidth, width - x *
                        tileWidth)) +
                    ", h=" + std::to_string(std::min(tileHeight, height - y *
                        tileHeight)));
            }
        }
    }

}
