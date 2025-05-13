#pragma once

#include "Network/Server/Session/Session.hpp"

#include <unordered_map>

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
        explicit Cluster(int heartbeatFrequency);

        void update(float dt);

        void addSlave(Session& session);
        void removeSlave(const Session& session);

    private:
        cluster::State _state;
        int _heartbeatFrequency;
        std::unordered_map<uint32_t, std::reference_wrapper<Session>> _slaves;
    };

}
