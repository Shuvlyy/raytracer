#include "Socket.hpp"

#include "logger/Logger.hpp"

#include "Exception/Exceptions/ClientDisconnected.hpp"
#include "Exception/Exceptions/StandardFunctionFail.hpp"

#include <format>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <sys/socket.h>

namespace raytracer::network
{

    Socket::Socket
    (
        const uint16_t port
    )
        : _address{}
    {
        this->_fd = socket(AF_INET, SOCK_STREAM, PROTOCOL_ANY);
        int opt = 0;

        if (this->_fd < 0) {
            throw exception::StandardFunctionFail("socket");
        }

        this->_address.sin_family = AF_INET;
        this->_address.sin_addr.s_addr = INADDR_ANY;
        this->_address.sin_port = htons(port);

        if (setsockopt(
            this->_fd,
            SOL_SOCKET,
            SO_REUSEADDR,
            &opt,
            sizeof(opt)
        ) < 0) {
            this->closeSocket();
            throw exception::StandardFunctionFail("setsockopt");
        }
    }

    void
    Socket::sendPacket
    (
        const ByteBuffer& data
    )
        const
    {
        if (this->_fd == -1 || data.empty()) {
            return;
        }

        try {
            auto& self = const_cast<Socket&>(*this);

            const auto size = static_cast<uint16_t>(data.size());
            const uint8_t header[HEADER_SIZE] = {
                static_cast<uint8_t>((size >> 8) & 0xFF),
                static_cast<uint8_t>(size & 0xFF)
            };

            self._writeBuffer.insert(
                self._writeBuffer.end(),
                header, header + HEADER_SIZE
            );
            self._writeBuffer.insert(
                self._writeBuffer.end(),
                data.begin(), data.end()
            );

            while (!self._writeBuffer.empty()) {
                pollfd pfd = {
                    .fd = this->_fd,
                    .events = POLLOUT,
                    .revents = 0
                };

                const int res = poll(&pfd, 1, 1000);

                if (res < 0) {
                    throw exception::StandardFunctionFail("poll (write)");
                }
                if (res == 0 || !(pfd.revents & POLLOUT)) {
                    continue;
                }

                const ssize_t bytesWritten = write(
                    this->_fd,
                    self._writeBuffer.data(),
                    self._writeBuffer.size()
                );

                if (bytesWritten < 0) {
                    throw exception::StandardFunctionFail("write");
                }

                self._writeBuffer.erase(
                    self._writeBuffer.begin(),
                    self._writeBuffer.begin() + bytesWritten
                );
            }

            LOG_DEBUG("Sent a packet of size " + std::to_string(size) + " (SFD: " + std::to_string(this->_fd) + ")");

            LOG_DEBUG("Packet Type: " + network::Packet::fromTypeToString(network::Packet::fromRawTypeToType(data[0])));
            LOG_DEBUG("Packet Content:");
            for (std::size_t i = 0; i < size; ++i) {
                LOG_DEBUG("\t" + std::to_string(i) + ". [" + std::format("0x{:02X}", data[i]) + "]");
            }
        }
        catch (exception::IException &exception) {
            LOG_ERR("Error while sending packet (SFD: " + std::to_string(this->_fd) + "): " + exception.what());
        }
    }

    ByteBuffer
    Socket::receivePacket
    ()
    {
        while (true) {
            if (this->_fd == -1) {
                return {}; // Empty ByteBuffer
            }

            pollfd pfd = {
                .fd = this->_fd,
                .events = POLLIN,
                .revents = 0
            };

            int res = poll(&pfd, 1, 1000);

            if (res < 0) {
                throw exception::StandardFunctionFail("poll (read)");
            }
            if (res == 0 || !(pfd.revents & POLLIN)) {
                continue;
            }

            char buffer[BUFFER_SIZE];
            const ssize_t bytesRead = read(this->_fd, buffer, BUFFER_SIZE);

            if (bytesRead < 0) {
                throw exception::StandardFunctionFail("read");
            }

            if (bytesRead == 0) {
                throw exception::ClientDisconnected(this->_fd);
            }

            this->_readBuffer.insert(
                this->_readBuffer.end(),
                buffer, buffer + bytesRead
            );

            while (this->_readBuffer.size() >= HEADER_SIZE) {
                const uint16_t packetSize =
                    (static_cast<uint16_t>(this->_readBuffer[0]) << 8) +
                    this->_readBuffer[1];

                if (this->_readBuffer.size() < static_cast<uint64_t>(packetSize) + HEADER_SIZE) {
                    break; // Wait for the entire packet to arrive
                }

                ByteBuffer packet(
                    this->_readBuffer.begin() + HEADER_SIZE,
                    this->_readBuffer.begin() + HEADER_SIZE + packetSize
                );

                this->_readBuffer.erase(
                    this->_readBuffer.begin(),
                    this->_readBuffer.begin() + HEADER_SIZE + packetSize
                );

                LOG_DEBUG("Client (SFD: " + std::to_string(this->_fd) + ") received a packet of size " + std::to_string(packetSize));

                LOG_DEBUG("Packet Type: " + Packet::fromTypeToString(Packet::fromRawTypeToType(packet[0])));
                for (std::size_t i = 0; i < packetSize; ++i) {
                    LOG_DEBUG("\t" + std::to_string(i) + ". [" + std::format("0x{:02X}", packet[i]) + "]");
                }

                return packet;
            }
        }
    }

    void
    Socket::closeSocket
    ()
    {
        /* If the socket has already been closed, don't continue */
        if (this->_fd == -1) {
            return;
        }

        close(this->_fd);
        this->_fd = -1;
    }

    void
    Socket::startListening
    (
        const int maxClients
    )
    {
        if (bind(
            this->_fd,
            reinterpret_cast<sockaddr*>(&this->_address),
            sizeof(this->_address)
        ) < 0) {
            this->closeSocket();
            throw exception::StandardFunctionFail("bind");
        }

        if (listen(this->_fd, maxClients) < 0) {
            close(this->_fd);
            throw exception::StandardFunctionFail("listen");
        }
    }

}
