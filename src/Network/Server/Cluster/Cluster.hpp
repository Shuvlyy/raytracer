#pragma once

#include <queue>

#include "Network/Server/Session/Session.hpp"
#include "Image/Image.hpp"

#include <unordered_map>

namespace raytracer::network
{
    class Server;
}

namespace raytracer::network::server
{

    namespace cluster
    {

        enum class State
        {
            LOADING,
            WAITING,
            READY,
            RENDERING,
            FINISHED
        };

    }

    class Cluster final
    {
    public:
        explicit Cluster(Server& server);

        void update();

        void addSlave(Session& session);
        void removeSlave(const Session& session);

        void setupImageOutput(const yml::Node& dimensionsNode);
        void setupImageOutput(uint32_t width, uint32_t height);

        void onTileFinished(Session& session);

        [[nodiscard]] cluster::State getState() const { return this->_state; }
        [[nodiscard]] std::unique_ptr<Image>& getResult() { return this->_result; }
        [[nodiscard]] int getHeartbeatFrequency() const
            { return this->_heartbeatFrequency; }

        void setState(const cluster::State state) { this->_state = state; }
        void setHeartbeatFrequency(const int heartbeatFrequency)
            { this->_heartbeatFrequency = heartbeatFrequency; }

    private:
        Server& _server;
        cluster::State _state;
        int _heartbeatFrequency;
        std::unordered_map<uint32_t, std::reference_wrapper<Session>> _slaves;
        std::unique_ptr<Image> _result;

        std::queue<renderer::Tile> _pendingTiles;
        std::unordered_map<uint32_t, renderer::Tile> _assignedTiles;

        void updateState();
        void updateSlavesData();

        void startRender();
        void checkRenderStatus();

        void createTiles(uint32_t width, uint32_t height);
    };

}
