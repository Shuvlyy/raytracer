#pragma once

#define PROTOCOL_ANY        0
#define BUFFER_SIZE         1024
#define DEFAULT_MAX_CLIENTS 192
#define HEADER_SIZE         2

#include "../../Packet/Packet.hpp"

#include <netinet/in.h>

namespace raytracer::network::server
{

    class Socket
    {
    public:
        /**
         * Creates a server socket.
         *
         * @param   port   Port to listen to
         */
        explicit Socket(uint16_t port);

        /**
         * Creates a client socket, from an existing file descriptor.
         *
         * @param   fd     File descriptor
         */
        explicit Socket(int fd);

        ~Socket() = default;

        /**
         * @param   data        Data to write
         */
        void sendPacket(const ByteBuffer &data) const;

        /**
         * Reads from the file descriptor and returns its content.
         */
        [[nodiscard]] ByteBuffer receivePacket();

        /**
         * Closes the socket: its file descriptor.
         */
        void closeSocket();

        /**
         * @return Socket's file descriptor
         */
        [[nodiscard]] int getFd() const;

        [[nodiscard]] sockaddr_in &getAddress();

        void startListening(int maxClients);

    protected:
        int _fd;
        sockaddr_in _address;
        std::vector<uint8_t> _bufferCache;
    };

}
