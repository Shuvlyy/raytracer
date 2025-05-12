#pragma once

#include "Packet/Manager.hpp"
#include "Session/Manager.hpp"
#include "Socket/Socket.hpp"

#include <string>
#include <vector>
#include <chrono>

#include <sys/poll.h>

namespace raytracer::network
{

    namespace server
    {

        struct Properties
        {
            uint16_t port;
            std::string configurationFilePath {};
        };

    }

    class Server final
    {
    public:
        explicit Server(server::Properties properties);
        ~Server();

        void start();
        void stop();

        void disconnectClient(
            const server::Socket &clientSocket
        );

        [[nodiscard]] server::Properties &getProperties() { return this->_properties; }
        [[nodiscard]] packet::server::Manager &getPacketManager() { return this->_packetManager; }
        [[nodiscard]] server::session::Manager &getSessionManager() { return this->_sessionManager; }
        [[nodiscard]] server::Socket &getServerSocket() { return this->_serverSocket; }
        [[nodiscard]] int getMaxClients() const { return this->_maxClients; }
        [[nodiscard]] bool isRunning() const { return this->_isRunning; }
        // [[nodiscard]] server::Game &getGame() { return this->_game; }

    private:
        server::Properties _properties;
        packet::server::Manager _packetManager;
        server::session::Manager _sessionManager;
        int _maxClients;
        bool _isRunning;
        server::Socket _serverSocket;
        std::vector<pollfd> _pollFds;
        // server::Game _game;

        void handleNewConnection();
        void handleClientRequest(server::Socket &clientSocket);
    };

}
