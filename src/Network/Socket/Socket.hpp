#pragma once

#define PROTOCOL_ANY        0
#define BUFFER_SIZE         1024
#define DEFAULT_MAX_CLIENTS 192
#define CHUNK_SIZE          (size_t) 32768
#define HEADER_SIZE         (size_t) 2

#include "../Packet/Packet.hpp"

#include <netinet/in.h>
#include <array>

namespace raytracer::network
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
         * Creates a client socket from an existing file descriptor.
         *
         * @param   fd     File descriptor
         */
        explicit Socket(const int fd)
            : _fd(fd), _address{} {}

        ~Socket() = default;

        /**
         * @param   data        Data to write
         */
        void sendPacket(const ByteBuffer& data) const;

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
        [[nodiscard]] int getFd() const { return this->_fd; }

        [[nodiscard]] sockaddr_in& getAddress() { return this->_address; }

        void startListening(int maxClients);

    protected:
        int _fd;
        sockaddr_in _address;
        ByteBuffer _readBuffer;
        ByteBuffer _writeBuffer;

        void flushWriteBuffer();
    };

    inline constexpr std::array<uint8_t, HEADER_SIZE> END_HEADER = { 0x00, 0x00 };

}
