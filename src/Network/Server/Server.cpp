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

namespace raytracer::network
{

    Server::Server
    (
        server::Properties properties
    )
        : _properties(std::move(properties)),
          _isRunning(false),
          _serverSocket(this->_properties.port)
          // _game({})
    {
        yml::Yml config(this->_properties.configurationFilePath);

        this->_settings = {
            config["serverName"].as<std::string>(),
            config["serverDescription"].as<std::string>(),
            static_cast<uint16_t>(config["maxClients"].as<int>())
        };

        // std::string configPath = properties.configurationFilePath;
        // if (!configPath.empty() && this->_config == nullptr) {
        //     throw exception::CouldNotOpenConfig();
        // }

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
            "\tScene filepath: \"" + this->_properties.sceneFilepath + "\""
        );

        while (true) {
            // this->_game.update(dt.count());

            // if (this->_game.getData().state == server::game::DEADASS) {
                // break;
            // }

            const int ret = poll(
                this->_pollFds.data(),
                this->_pollFds.size(),
                0
            );

            if (!this->_isRunning) {
                break;
            }

            if (ret < 0) {
                throw exception::StandardFunctionFail("poll");
            }

            for (const pollfd& fd : this->_pollFds) {
                server::Socket clientSocket(fd.fd);

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

        const server::Socket clientSocket(clientFd);

        this->_pollFds.push_back({
            .fd = clientSocket.getFd(),
            .events = POLLIN,
            .revents = 0
        });

        this->_sessionManager.createSession(clientSocket);

        server::Session &session = this->_sessionManager.getSession(clientSocket);
        // this->_game.addPlayer(session);
        //
        // const server::packet::Hi hiPacket(
        //     session.getId(),
        //     this->_game.getSettings().getMap()
        // );

        // session.getControlSocket().sendPacket(hiPacket.serialize());

        LOG_INFO("Client (SFD: " + std::to_string(clientFd) + ") connected.");
    }

    void
    Server::handleClientRequest
    (
        server::Socket& clientSocket
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
        const server::Socket& clientSocket
    )
    {
        const server::Session& session =
            this->_sessionManager.getSession(clientSocket);

        // this->_game.removePlayer(session);

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

}
