#pragma once

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

        void setupImageOutput(uint32_t width, uint32_t height);

        [[nodiscard]] std::unique_ptr<Image>& getResult() { return this->_result; }

        [[nodiscard]] int getHeartbeatFrequency() const
            { return this->_heartbeatFrequency; }
        void setHeartbeatFrequency(const int heartbeatFrequency)
            { this->_heartbeatFrequency = heartbeatFrequency; }
        void addFinishedTile() { ++this->_finishedTiles; }

    private:
        Server& _server;
        cluster::State _state;
        int _heartbeatFrequency;
        std::unordered_map<uint32_t, std::reference_wrapper<Session>> _slaves;
        std::unique_ptr<Image> _result;
        std::vector<renderer::Tile> _tiles;
        size_t _nextTile;
        size_t _finishedTiles;

        void updateState();
        void updateSlavesData();

        void startRender();
        void checkRenderStatus();

        void createTiles(uint32_t width, uint32_t height);
    };

}
