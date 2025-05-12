#include "Network/Server/Socket/Socket.hpp"

#include "Logger.hpp"

#include "Exception/Exceptions/ClientDisconnected.hpp"
#include "Exception/Exceptions/StandardFunctionFail.hpp"

#include <format>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

namespace raytracer::network::server
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

    Socket::Socket
    (
        const int fd
    )
        : _fd(fd), _address{}
    {}

    void
    Socket::sendPacket
    (
        const ByteBuffer &data
    )
        const
    {
        if (this->_fd == -1 || data.empty()) {
            return;
        }

        try {
            const auto size = static_cast<uint16_t>(data.size());
            const uint8_t header[HEADER_SIZE] = {
                static_cast<uint8_t>((size >> 8) & 0xFF),
                static_cast<uint8_t>(size & 0xFF)
            };

            if (write(this->_fd, header, HEADER_SIZE) == -1) {
                throw exception::StandardFunctionFail("write");
            }

            if (write(this->_fd, data.data(), data.size()) == -1) {
                throw exception::StandardFunctionFail("write");
            }

            LOG_DEBUG("Sent a packet of size " + std::to_string(size) + " (SFD: " + std::to_string(this->_fd) + ")");

            LOG_DEBUG("Packet Type: " + network::Packet::fromTypeToString(network::Packet::fromRawTypeToType(data[0])));
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

            char buffer[BUFFER_SIZE];
            const ssize_t bytesRead = read(this->_fd, buffer, BUFFER_SIZE);

            if (bytesRead < 0) {
                throw exception::StandardFunctionFail("read");
            }

            if (bytesRead == 0) {
                throw exception::ClientDisconnected(this->_fd);
            }

            this->_bufferCache.insert(this->_bufferCache.end(), buffer, buffer + bytesRead);

            while (this->_bufferCache.size() >= HEADER_SIZE) {
                const uint16_t packetSize =
                    (static_cast<uint16_t>(this->_bufferCache[0]) << 8) + this->_bufferCache[1];

                if (this->_bufferCache.size() < static_cast<uint64_t>(packetSize) + HEADER_SIZE) {
                    break; // Wait for the entire packet to arrive
                }

                ByteBuffer packet(
                    this->_bufferCache.begin() + HEADER_SIZE,
                    this->_bufferCache.begin() + HEADER_SIZE + packetSize
                );

                this->_bufferCache.erase(
                    this->_bufferCache.begin(),
                    this->_bufferCache.begin() + HEADER_SIZE + packetSize
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

    int
    Socket::getFd
    ()
        const
    {
        return this->_fd;
    }

    void
    Socket::startListening
    (
        const int maxClients
    )
    {
        if (bind(
            this->_fd,
            reinterpret_cast<sockaddr *>(&this->_address),
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

    sockaddr_in &
    Socket::getAddress
    ()
    {
        return this->_address;
    }

}
