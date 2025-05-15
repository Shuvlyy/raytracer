#include "Cluster.hpp"

#include "logger/Logger.hpp"
#include "Network/Packet/Packets/Workslave.hpp"
#include "Network/Server/Server.hpp"

namespace raytracer::network::server
{

    Cluster::Cluster
    (
        Server& server
    )
        : _server(server),
          _state(cluster::State::WAITING),
          _heartbeatFrequency(0)
    {}

    void
    Cluster::update
    (
        const float dt
    )
    {
        this->updateState();

        if (this->_state == cluster::State::WAITING) {
            return;
        }

        this->updateSlavesData();

        if (this->_state == cluster::State::READY) {
            LOG_DEBUG("Rendering lol");
            this->_state = cluster::State::RENDERING;

            for (auto& [_, s] : this->_slaves) {
                packet::Workslave p(this->_server.getConfig().getRawContent(), 0, 0, 640, 480);

                s.get().getControlSocket().sendPacket(p.serialize());
            }
        }
    }

    void
    Cluster::updateState
    ()
    {
        if (this->_state == cluster::State::WAITING) {
            if (this->_slaves.size() == this->_server.getSettings().maxClients) {
                this->_state = cluster::State::READY;
            }
        }
    }

    void Cluster::updateSlavesData()
    {
        using namespace std::chrono;

        for (auto& [_, s] : this->_slaves) {
            Session& slave = s.get();
            Socket& slaveSocket = slave.getControlSocket();

            auto now = steady_clock::now();

            LOG_DEBUG("Updating slave " + std::to_string(slave.getId()));

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
        if (!this->_slaves.contains(session.getId())) {
            return;
        }
        this->_slaves.erase(session.getId());
    }

}
