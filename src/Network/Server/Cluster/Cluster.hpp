#pragma once

#include "Network/Server/Session/Session.hpp"

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
            RENDERING
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

        [[nodiscard]] int getHeartbeatFrequency() const
            { return this->_heartbeatFrequency; }
        void setHeartbeatFrequency(const int heartbeatFrequency)
            { this->_heartbeatFrequency = heartbeatFrequency; }

    private:
        Server& _server;
        cluster::State _state;
        int _heartbeatFrequency;
        std::unordered_map<uint32_t, std::reference_wrapper<Session>> _slaves;
        std::unique_ptr<Image> _result;

        void updateState();
        void updateSlavesData();
    };

}
