#pragma once

#include "Network/Packet/Packet.hpp"
#include "Network/Client/Packet/IHandler.hpp"

#include <memory>
#include <unordered_map>

namespace raytracer::network::packet
{

    class Manager final
    {
    public:
        explicit Manager();

        void dispatchPacket(
            const Packet &packet,
            Client &cli
        ) const;

    private:
        std::unordered_map<Type, std::unique_ptr<IHandler>> _handlers;

        IHandler *getHandler(Type type) const;

        void registerHandlers();
        void registerHandler(std::unique_ptr<IHandler> handler);
    };

}
