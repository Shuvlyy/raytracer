#pragma once

#include "Network/Packet/Packet.hpp"
#include "Network/Client/Channel/Channel.hpp"
#include "Network/Socket/Socket.hpp"

#define POLL_TIMEOUT 200

namespace raytracer::app {

    class Client;

}

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

            void run(app::Client &cli);
            void stop() { this->_running = false; }

            [[nodiscard]] Socket getSocket() const {return this->_socket;}
            [[nodiscard]] int getFd() const {return this->_socket.getFd();}

            void sendPacket(const std::unique_ptr<Packet> &packet);
            std::unique_ptr<Packet> receivePacket();

            /**
             * @return A std::unique_ptr<Packet> to the packet to process
             * @return A nullptr if there is no packet left in the Queue
             * @note Returned packets are not processed either deserialized
             */
            std::unique_ptr<Packet> popPacket() { return this->_toProcess.pop(); }

             /**
              * @param packet a std::unique_ptr<Packet> to add in the waiting queue to be send
              * @note add a packet in a queue
             */
            void pushPacket(std::unique_ptr<Packet> packet) { this->_toSend.push(std::move(packet)); }

            bool hasPacketToProcess() const { return !this->_toProcess.empty(); }

        private:
            Socket _socket;
            Channel _toSend;
            Channel _toProcess;
            bool _running {false};
    };

}
