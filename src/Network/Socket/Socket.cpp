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
        int opt = 1;

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
    {
        if (this->_fd == -1 || data.empty()) {
            LOG_WARN("Skipping send: invalid fd or empty data");
            return;
        }

        try {
            size_t offset = 0;

            while (offset < data.size()) {
                const size_t chunkSize = std::min(CHUNK_SIZE, data.size() - offset);
                std::array<uint8_t, HEADER_SIZE> header = {
                    static_cast<uint8_t>((chunkSize >> 8) & 0xFF),
                    static_cast<uint8_t>(chunkSize & 0xFF)
                };

                this->_writeBuffer.insert(
                    this->_writeBuffer.end(),
                    header.data(),
                    header.data() + HEADER_SIZE
                );

                this->_writeBuffer.insert(
                    this->_writeBuffer.end(),
                    data.begin() + offset,
                    data.begin() + offset + chunkSize
                );

                offset += chunkSize;
            }

            this->_writeBuffer.insert(
                this->_writeBuffer.end(),
                END_HEADER.begin(),
                END_HEADER.end()
            );

            this->flushWriteBuffer();

            LOG_DEBUG("Sent a packet of size " + std::to_string(data.size()) + " (SFD: " + std::to_string(this->_fd) + ")");

            if (!data.empty()) {
                const std::string type = Packet::fromTypeToString(Packet::fromRawTypeToType(data[0]));
                // std::string content;
                // for (uint16_t i = 0; i < data.size(); ++i) {
                // content += std::format("{:02X}{}", data[i], i == data.size() - 1 ? "" : " ");
                // }

                LOG_DEBUG(
                    "Packet details:\n"
                    "\tType: " + type + "\n"
                    // "\tContent: [" + content + "]"
                );
            }
        }
        catch (exception::IException &exception) {
            LOG_ERR("Error while sending packet (SFD: " + std::to_string(this->_fd) + "): " + exception.what());
        }
    }

    ByteBuffer
    Socket::receivePacket()
    {
        ByteBuffer packet;

        while (true) {
            if (this->_fd == -1) {
                return {}; // Empty ByteBuffer
            }

            pollfd pfd = {
                .fd = this->_fd,
                .events = POLLIN,
                .revents = 0
            };

            int res = poll(&pfd, 1, POLL_TO);

            if (res < 0) {
                throw exception::StandardFunctionFail("poll (read)");
            }
            if (pfd.revents & POLLHUP) {
                LOG_WARN("POLLHUP");
                throw exception::ClientDisconnected(this->_fd);
            }
            if (pfd.revents & (POLLERR | POLLNVAL)) {
                LOG_WARN("POLLERR OR NVAL");
                return {};
            }
            if (res == 0 || !(pfd.revents & POLLIN)) {
                LOG_WARN(std::format("Continuing, res={} and revents={}", res, pfd.revents));
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
                uint16_t chunkSize =
                    (static_cast<uint16_t>(this->_readBuffer[0]) << 8)
                    | static_cast<uint16_t>(this->_readBuffer[1]);

                LOG_DEBUG("Parsing chunkSize: " + std::to_string(chunkSize) + " bytes");

                if (chunkSize == 0) {
                    this->_readBuffer.erase(
                        this->_readBuffer.begin(),
                        this->_readBuffer.begin() + HEADER_SIZE
                    );

                    LOG_DEBUG("Client (SFD: " + std::to_string(this->_fd) + ") received a packet of size " + std::to_string(packet.size()));

                    if (!packet.empty()) {
                        const std::string type = Packet::fromTypeToString(Packet::fromRawTypeToType(packet[0]));
                        // std::string content;

                        // for (uint16_t i = 0; i < packet.size(); ++i) {
                            // content += std::format("{:02X}{}", packet[i], i == packet.size() - 1 ? "" : " ");
                        // }

                        LOG_DEBUG(
                            "Packet details:\n"
                            "\tType: " + type + "\n"
                            // "\tContent: [" + content + "]"
                        );
                    }

                    return packet;
                }

                if (this->_readBuffer.size() < HEADER_SIZE + chunkSize) {
                    LOG_DEBUG("Waiting for more data...");
                    break; // Wait for the entire packet to arrive
                }

                LOG_DEBUG("Inserting in packet");

                packet.insert(
                    packet.end(),
                    this->_readBuffer.begin() + HEADER_SIZE,
                    this->_readBuffer.begin() + HEADER_SIZE + chunkSize
                );

                this->_readBuffer.erase(
                    this->_readBuffer.begin(),
                    this->_readBuffer.begin() + HEADER_SIZE + chunkSize
                );
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

    void
    Socket::flushWriteBuffer()
    {
        size_t retries = 0;

        while (!this->_writeBuffer.empty()) {
            if (++retries > MAX_SEND_RETRIES) {
                LOG_WARN("Max retries reached while sending data (SFD: " + std::to_string(this->_fd) + ")");
                break;
            }

            pollfd pfd = {
                .fd = this->_fd,
                .events = POLLOUT,
                .revents = 0
            };

            const int res = poll(&pfd, 1, POLL_TO);

            if (res < 0) {
                throw exception::StandardFunctionFail("poll (write)");
            }
            if (pfd.revents & POLLHUP) {
                throw exception::ClientDisconnected(this->_fd);
            }
            if (res == 0 || !(pfd.revents & POLLOUT)) {
                continue;
            }

            LOG_DEBUG("Write buffer size before write: " + std::to_string(this->_writeBuffer.size()));

            const ssize_t bytesWritten = write(
                this->_fd,
                this->_writeBuffer.data(),
                this->_writeBuffer.size()
            );

            LOG_DEBUG("Bytes written: " + std::to_string(bytesWritten));

            if (bytesWritten < 0) {
                throw exception::StandardFunctionFail("write");
            }

            this->_writeBuffer.erase(
                this->_writeBuffer.begin(),
                this->_writeBuffer.begin() + bytesWritten
            );

            LOG_DEBUG("Wrote " + std::to_string(bytesWritten) + " bytes (SFD: " + std::to_string(this->_fd) + ")");
        }
    }


}
