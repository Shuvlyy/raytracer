#include "Server.hpp"
#include "logger/Logger.hpp"
#include "yml/Yml.hpp"

#include "Exception/Exceptions/ClientDisconnected.hpp"
#include "Exception/Exceptions/StandardFunctionFail.hpp"

#include <chrono>
#include <utility>
#include <cmath>
#include <thread>

#include <netinet/in.h>

#include "Renderer/Renderer.hpp"

namespace raytracer::network
{

    Server::Server
    (
        server::Properties properties
    )
        : _properties(std::move(properties)),
          _sceneConfig(this->_properties.sceneFilepaths.at(0)),
          _isRunning(false),
          _serverSocket(this->_properties.port),
          _packetManager(*this),
          _cluster(*this),
          _currentlyProcessingScene(0)
    {
        for (const auto& sceneFilepath : properties.sceneFilepaths) {
            LOG_DEBUG("Found filepath: " + sceneFilepath);
        }

        auto serverConfig = yml::Yml(this->_properties.configurationFilePath);

        this->_settings = {
            serverConfig["serverName"].as<std::string>(),
            serverConfig["serverDescription"].as<std::string>(),
            static_cast<uint16_t>(serverConfig["maxClients"].as<int>()),
        };

        this->_properties.heartbeatFrequency = serverConfig["heartbeatFrequency"].as<int>();
        this->_cluster.setHeartbeatFrequency(this->_properties.heartbeatFrequency);

        if (this->_properties.tileSize == -1) {
            this->_properties.tileSize = DEFAULT_CLUSTER_TILE_SIZE;
        }

        this->_serverSocket.startListening(this->_settings.maxClients);

        this->_pollFds.push_back({
            .fd = this->_serverSocket.getFd(),
            .events = POLLIN,
            .revents = 0
        });
    }

    Server::~Server
    ()
    {
        LOG_INFO("Shutting down server...");

        this->_isRunning = false;
        this->_sessionManager.closeAllSessions();
        this->_serverSocket.closeSocket();

        LOG_INFO("Server shut down. Bye!");
    }

    void
    Server::start
    ()
    {
        this->_isRunning = true;

        LOG_INFO("Server started!");
        LOG_DEBUG(
            "Server settings:\n"
            "\tPort: " + std::to_string(this->_properties.port) + "\n"
            "\tConfiguration file: \"" + (
                this->_properties.configurationFilePath.empty()
                    ? "NONE"
                    : this->_properties.configurationFilePath
            ) + "\"\n"
            "\tName: \"" + this->_settings.serverName + "\"\n"
            "\tDescription: \"" + this->_settings.serverDescription + "\"\n"
            "\tMax clients: " + std::to_string(this->_settings.maxClients) + "\n"
            "\tScene filepaths:\n" +
            [&] {
                std::string res;
                for (const auto& sceneFilepath: this->_properties.sceneFilepaths)
                    res += "\t\t- \"" + sceneFilepath + "\"\n";
                return res;
            }() +
            "\tHeartbeat frequency: " + std::to_string(this->_properties.heartbeatFrequency) + "s"
        );

        // const yml::Node res = this->_sceneConfig["camera"]["resolution"];
        // this->_cluster.setupImageOutput(res);

        while (true) {
            const server::cluster::State clusterState = this->_cluster.getState();

            if (clusterState == server::cluster::State::FINISHED ||
                clusterState == server::cluster::State::LOADING
            ) {
                if (clusterState == server::cluster::State::FINISHED) {
                    this->_currentlyProcessingScene++;
                }

                if (this->_properties.sceneFilepaths.size() <= this->_currentlyProcessingScene) {
                    this->stop();
                    return;
                }

                // this->_sceneConfig = yml::Yml(this->_properties.sceneFilepaths.at(this->_currentlyProcessingScene));

                this->_sceneConfig.loadFromFilepath(this->_properties.sceneFilepaths.at(this->_currentlyProcessingScene));

                if (!this->checkCurrentScene()) {
                    continue;
                }

                const yml::Node node = this->_sceneConfig["camera"]["resolution"];
                this->_cluster.setupImageOutput(node);

                if (clusterState == server::cluster::State::FINISHED) {
                    this->_cluster.setState(server::cluster::State::READY);
                } else if (clusterState == server::cluster::State::LOADING) {
                    this->_cluster.setState(server::cluster::State::WAITING);
                }
            }

            this->_cluster.update();

            const int ret = poll(
                this->_pollFds.data(),
                this->_pollFds.size(),
                this->_properties.heartbeatFrequency * 1000
            );

            if (!this->_isRunning) {
                break;
            }

            if (ret < 0) {
                throw exception::StandardFunctionFail("poll");
            }

            for (const pollfd& fd : this->_pollFds) {
                Socket clientSocket(fd.fd);

                /* Hmm, not really useful. */
                /*if (fd.revents & POLLHUP) {
                    this->disconnectClient(clientSocket);
                    continue;
                }*/

                if (!(fd.revents & POLLIN)) {
                    continue;
                }

                if (fd.fd == this->_serverSocket.getFd()) {
                    this->handleNewConnection();
                } else {
                    this->handleClientRequest(clientSocket);
                }
            }
        }
    }

    void
    Server::stop
    ()
    {
        if (!this->_isRunning) {
            return;
        }

        this->_isRunning = false;
        this->_sessionManager.closeAllSessions();
        this->_serverSocket.closeSocket();
    }

    void
    Server::handleNewConnection
    ()
    {
        sockaddr_in clientAddr {};
        socklen_t clientAddrSize = sizeof(clientAddr);
        const int clientFd = accept(
            this->_serverSocket.getFd(),
            reinterpret_cast<sockaddr*>(&clientAddr),
            &clientAddrSize
        );

        if (clientFd < 0) {
            throw exception::StandardFunctionFail("accept");
        }

        const Socket clientSocket(clientFd);

        this->_pollFds.push_back({
            .fd = clientSocket.getFd(),
            .events = POLLIN,
            .revents = 0
        });

        this->_sessionManager.createSession(clientSocket);

        server::Session& session = this->_sessionManager.getSession(clientSocket);
        this->_cluster.addSlave(session);

        LOG_INFO("Client (SFD: " + std::to_string(clientFd) + ") connected.");
    }

    void
    Server::handleClientRequest
    (
        Socket& clientSocket
    )
    {
        try {
            const auto rawPacket = clientSocket.receivePacket();

            if (rawPacket.empty()) {
                return;
            }

            LOG_DEBUG("Received packet client (SFD: " + std::to_string(clientSocket.getFd()) + ")");

            const std::unique_ptr<Packet> packet = Packet::fromByteBuffer(rawPacket);

            server::Session& session = this->getSessionManager()
                .getSession(clientSocket);

            this->_packetManager.dispatchPacket(*packet, session);
        }
        catch (exception::ClientDisconnected&) {
            this->disconnectClient(clientSocket);
        }
        catch (exception::IException& exception) {
            LOG_ERR(
                "Error while handling Client's request (SFD: " +
                std::to_string(clientSocket.getFd()) + "): " +
                std::string(exception.what())
            );
        }
    }

    void
    Server::disconnectClient
    (
        const Socket& clientSocket
    )
    {
        const server::Session& session =
            this->_sessionManager.getSession(clientSocket);

        this->_cluster.removeSlave(session);

        for (size_t k = 0; k < this->_pollFds.size(); k++) {
            pollfd& fd = this->_pollFds.at(k);

            if (fd.fd == clientSocket.getFd()) {
                this->_pollFds.erase(this->_pollFds.begin() + static_cast<long>(k));
                break;
            }
        }

        this->_sessionManager.closeSession(clientSocket);

        LOG_INFO("Client (SFD: " + std::to_string(clientSocket.getFd()) + ") disconnected.");
    }

    bool
    Server::checkCurrentScene()
    {
        try {
            /*
             * Instancing a Renderer class with the config, so that if
             * the config is invalid, an exception will be thrown
             * automatically by the constructor of the Renderer class.
             */
            Renderer r(this->_sceneConfig);
            return true;
        }
        catch (...) {
            LOG_CRIT("Invalid scene file, skipping... (" + this->_properties.sceneFilepaths.at(this->_currentlyProcessingScene) + ")");
            // this->_sceneConfig.dump();
            return false;
        }
    }

}
