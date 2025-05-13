#pragma once

#define DEFAULT_HEARTBEAT_FREQUENCY 1

#include "Packet/Manager.hpp"
#include "Session/Manager.hpp"
#include "Network/Socket/Socket.hpp"
#include "Cluster/Cluster.hpp"

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
            int heartbeatFrequency = DEFAULT_HEARTBEAT_FREQUENCY;
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
        [[nodiscard]] server::Cluster& getCluster() { return this->_cluster; }

    private:
        server::Properties _properties;
        server::Settings _settings;
        bool _isRunning;
        Socket _serverSocket;
        std::vector<pollfd> _pollFds;
        packet::server::Manager _packetManager;
        server::session::Manager _sessionManager;
        server::Cluster _cluster;

        void handleNewConnection();
        void handleClientRequest(Socket &clientSocket);
    };

}
