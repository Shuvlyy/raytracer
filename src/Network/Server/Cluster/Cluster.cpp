#include "Cluster.hpp"

#include "logger/Logger.hpp"

namespace raytracer::network::server
{

    Cluster::Cluster
    (
        const int heartbeatFrequency
    )
        : _state(cluster::State::WAITING),
          _heartbeatFrequency(heartbeatFrequency)
    {}

    void
    Cluster::update
    (
        const float dt
    )
    {
        using namespace std::chrono;

        LOG_DEBUG("updating cluster");
        // if (this->_state == cluster::State::WAITING) {
            // return;
        // }

        for (auto& [_, s] : this->_slaves) {
            Session& slave = s.get();
            Socket& slaveSocket = slave.getControlSocket();

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
        Session &session
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
        const Session &session
    )
    {
        if (!this->_slaves.contains(session.getId())) {
            return;
        }
        this->_slaves.erase(session.getId());
    }

}
