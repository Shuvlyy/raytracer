#pragma once

#include "Session.hpp"
#include "Network/Server/Socket/Socket.hpp"

#include <vector>
#include <unordered_map>

namespace raytracer::network::server::session
{

    class Manager final
    {
    public:
        explicit Manager();

        /**
         * Creates a new session for a user.
         *
         * @param   clientSocket    Client's socket
         */
        void createSession(const Socket &clientSocket);

        /**
         * Closes an active session.
         *
         * @param   socketFd    File descriptor of the socket of the session to close
         * @param   session     Session to close
         */
        void closeSession(int socketFd, Session &session);

        /**
         * Closes an active session.
         *
         * @param   clientSocket    Client's socket
         */
        void closeSession(const Socket &clientSocket);

        /**
         * Closes all active sessions.
         */
        void closeAllSessions();

        /**
         * @return  true if client's socket has an active session. Otherwise, false.
         */
        bool hasSession(const Socket &clientSocket) const;

        std::vector<Session *> getSessions();

        /**
         * @return  Session associated with the socket
         */
        Session &getSession(const Socket &clientSocket);

    private:
        uint32_t _sessionsCreated;

        /*                      v Session associated with the Client FD */
        std::unordered_map<int, Session> _sessions;
        /*                 ^ Client FD                                  */
    };

}
