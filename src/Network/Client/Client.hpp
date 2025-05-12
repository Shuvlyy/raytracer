#pragma once

#include "Channel/Channel.hpp"
#include "Network/Packet/Packet.hpp"
#include "Network/Socket.hpp"

#include <vector>

#define POLL_TIMEOUT 200

namespace raytracer::network
{

    class Client final
    {
        public:

            /**
             *
             * @param serverAddress The address of the server
             * @param port The port on which initiate the connection
             *
             */
            Client(const std::string &serverAddress, int port);
            Client(Client &oth) = delete;

            ~Client();

            /**
             * runs the network part of the client and handle received packets.
             * This method modifies the content returned by getPlayers() the value returned by getDistance()
             * and the value returned by getLatency()
             * @note this method is aimed to be used in another thread than the main program
             */
            void run();

            [[nodiscard]] Socket getSocket() const {return this->_socket;}
            [[nodiscard]] int getFd() const {return this->_socket.getFd();}

            void sendPacket(std::unique_ptr<Packet> packet) { this->_toSend.push(std::move(packet)); }
            [[nodiscard]] std::unique_ptr<Packet> getPacketToProcess() { return this->_toProcess.pop(); }

            [[nodiscard]] bool hasPacketToProcess() const { return !this->_toProcess.empty(); }

        private:
            Socket _socket;
            Channel _toProcess;
            Channel _toSend;
    };

}
