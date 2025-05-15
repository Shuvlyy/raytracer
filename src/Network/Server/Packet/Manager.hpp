#pragma once

#include "Network/Packet/Packet.hpp"
#include "IHandler.hpp"

#include <memory>
#include <unordered_map>

namespace raytracer::network::packet::server
{

    class Manager final
    {
    public:
        explicit Manager(Server& server);

        void dispatchPacket(
            const Packet& packet,
            network::server::Session& session
        ) const;

    private:
        Server& _server;
        std::unordered_map<Type, std::unique_ptr<IHandler>> _handlers;

        IHandler* getHandler(Type type) const;

        void registerHandlers();
        void registerHandler(std::unique_ptr<IHandler> handler);
    };

}
