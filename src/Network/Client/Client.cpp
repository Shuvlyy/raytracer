#include "Exception/Exceptions/SocketFail.hpp"
#include "Exception/Exceptions/ServerDisconnected.hpp"
#include "Exception/Exceptions/ConnectionFail.hpp"
#include "Exception/Exceptions/StandardFunctionFail.hpp"
#include "Exception/Exceptions/EmptyPacket.hpp"

#include "Client.hpp"
#include "logger/Logger.hpp"
#include "Network/Socket/Socket.hpp"

#include <cstring>
#include <string>

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
        : _socket(0)
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
    Client::sendPacket
    (
        const std::unique_ptr<Packet> &packet
    )
    {
        if (!packet)
            return;
        this->_socket.sendPacket(packet->serialize());
    }

    std::unique_ptr<Packet>
    Client::receivePacket
    ()
    {
        const auto rawPacket = this->_socket.receivePacket();

        if (rawPacket.empty()) {
            throw exception::EmptyPacket();
        }
        LOG_DEBUG("Received Packet");
        return Packet::fromByteBuffer(rawPacket);
    }

    void
    Client::run()
    {
        pollfd pfd{};

        pfd.fd = this->_socket.getFd();
        pfd.events = POLLIN;

        while (this->_running) {
            try {
                if (!this->_toSend.empty()) {
                    sendPacket(this->_toSend.pop());
                }
                if (poll(&pfd, 1, POLL_TIMEOUT) < 0) {
                    throw exception::StandardFunctionFail("poll");
                }
                if (!(pfd.revents & POLLIN)) {
                    continue;
                }
                this->_toProcess.push(receivePacket());
            }
            catch (exception::ServerDisconnected &) {
                LOG_FATAL("Server disconnected");
            }
            catch (exception::IException &exception) {
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
