#include "Manager.hpp"

#include "Network/Packet/Packets/Kiss.hpp"

namespace raytracer::network::server::session
{

    Manager::Manager
    ()
        : _sessionsCreated(0)
    {}

    void
    Manager::createSession
    (
        const Socket& clientSocket
    )
    {
        if (this->hasSession(clientSocket)) {
            return;
        }

        const int clientFd = clientSocket.getFd();

        this->_sessions.emplace(
            clientFd,
            Session(clientFd, this->_sessionsCreated++)
        );
    }

    void
    Manager::closeSession
    (
        const int socketFd,
        Session& session
    )
    {
        session.getControlSocket().closeSocket();
        this->_sessions.erase(socketFd);
    }

    void
    Manager::closeSession
    (
        const Socket& clientSocket
    )
    {
        const int socketFd = clientSocket.getFd();
        Session& session = this->_sessions.at(socketFd);

        this->closeSession(socketFd, session);
    }

    void
    Manager::closeAllSessions
    ()
    {
        for (auto& [fd, session] : this->_sessions) {
            packet::Kiss k;
            session.getControlSocket().sendPacket(k.serialize());
            session.getControlSocket().closeSocket();
        }
        this->_sessions.clear();
    }

    bool
    Manager::hasSession
    (
        const Socket& clientSocket
    )
        const
    {
        return this->_sessions.contains(clientSocket.getFd());
    }

    std::vector<Session*>
    Manager::getSessions
    ()
    {
        std::vector<Session*> result;

        for (auto& [_, session] : this->_sessions) {
            result.push_back(&session);
        }
        return result;
    }

    Session&
    Manager::getSession
    (
        const Socket& clientSocket
    )
    {
        return this->_sessions.find(clientSocket.getFd())->second;
    }

}
