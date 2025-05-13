#pragma once

#include "Packet/Manager.hpp"
#include "Session/Manager.hpp"
#include "Network/Socket/Socket.hpp"

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
            std::string sceneFilepath {};
        };

        struct Settings
        {
            std::string serverName;
            std::string serverDescription;
            uint16_t maxClients;
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
            const Socket& clientSocket
        );

        [[nodiscard]] server::Properties& getProperties() { return this->_properties; }
        [[nodiscard]] server::Settings& getSettings() { return this->_settings; }
        [[nodiscard]] bool isRunning() const { return this->_isRunning; }
        [[nodiscard]] Socket& getServerSocket() { return this->_serverSocket; }
        [[nodiscard]] packet::server::Manager& getPacketManager() { return this->_packetManager; }
        [[nodiscard]] server::session::Manager& getSessionManager() { return this->_sessionManager; }
        // [[nodiscard]] server::Game &getGame() { return this->_game; }

    private:
        server::Properties _properties;
        server::Settings _settings;
        bool _isRunning;
        Socket _serverSocket;
        std::vector<pollfd> _pollFds;
        packet::server::Manager _packetManager;
        server::session::Manager _sessionManager;
        // server::Game _game;

        void handleNewConnection();
        void handleClientRequest(Socket &clientSocket);
    };

}
