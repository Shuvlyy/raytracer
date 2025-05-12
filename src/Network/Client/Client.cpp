#include "Exception/Exceptions/Network/ConnectionFail.hpp"
#include "Exception/Exceptions/Network/SocketFail.hpp"
#include "Exception/Exceptions/Network/StandardFunctionFail.hpp"
#include "Exception/Exceptions/Network/ServerDisconnected.hpp"
#include "Exception/Exceptions/Network/EmptyPacket.hpp"

#include "Client.hpp"
#include "Logger.hpp"

#include <cstring>

#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

namespace raytracer::network
{

    Client::Client
    (
        const std::string &serverAddress,
        const int port
    )
    {
        sockaddr_in addr {};

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = inet_addr(serverAddress.c_str());
        addr.sin_port = htons(port);

        int serverFd = socket(AF_INET, SOCK_STREAM, PROTOCOL_ANY);
        if (serverFd < 0) {
            throw exception::SocketFail(strerror(errno));
        }
        if (connect(serverFd, reinterpret_cast<sockaddr *>(&addr), sizeof(addr))) {
            close(serverFd);
            throw exception::ConnectionFail(port, serverAddress, strerror(errno));
        }
        this->_socket = Socket(serverFd);

        LOG_INFO("Server successfully connected to " + serverAddress + " on port " + std::to_string(port));
    }

    Client::~Client
    ()
    {
        this->_socket.closeSocket();
        LOG_INFO("Disconnecting from server.");
    }

    void
    Client::run
    ()
    {
        pollfd pfd{};

        pfd.fd = this->_socket.getFd();
        pfd.events = POLLIN;

        while (true) {
            try {
                if (!this->_toSend.empty()) {
                    this->_socket.sendPacket(this->_toSend.pop()->serialize());
                }
                if (poll(&pfd, 1, POLL_TIMEOUT) < 0) {
                    throw exception::StandardFunctionFail("poll");
                }
                if (!(pfd.revents & POLLIN)) {
                    continue;
                }
                const auto rawPacket = this->_socket.receivePacket();
                if (rawPacket.empty()) {
                    throw network::exception::EmptyPacket();
                }
                LOG_DEBUG("Received packet client (SFD: " + std::to_string(this->_socket.getFd()) + ")");
                this->_toProcess.push(Packet::fromByteBuffer(rawPacket));
            }
            catch (exception::ServerDisconnected &) {
                LOG_FATAL("Server disconnected");
            }
            catch (IException &exception) {
                LOG_ERR(
                    "Error while handling Client `" +
                    std::to_string(this->_socket.getFd()) +
                    "`'s requests: " +
                    std::string(exception.what())
                );
            }
        }
    }

}
