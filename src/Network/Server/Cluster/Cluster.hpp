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

        void update(float dt);

        void addSlave(Session& session);
        void removeSlave(const Session& session);

        [[nodiscard]] int getHeartbeatFrequency() const
            { return this->_heartbeatFrequency; }
        void setHeartbeatFrequency(const int heartbeatFrequency)
            { this->_heartbeatFrequency = heartbeatFrequency; }

    private:
        Server& _server;
        cluster::State _state;
        int _heartbeatFrequency;
        std::unordered_map<uint32_t, std::reference_wrapper<Session>> _slaves;

        void updateState();
    };

}
