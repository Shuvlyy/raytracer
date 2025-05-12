#pragma once

#include "../../Packet/Packet.hpp"
#include "../../Packet/IHandler.hpp"

#include <memory>
#include <unordered_map>

namespace raytracer::server::packet
{

    class Manager final
    {
    public:
        explicit Manager();

        void dispatchPacket(
            const Packet &packet,
            Session &session
        ) const;

    private:
        std::unordered_map<Type, std::unique_ptr<IHandler>> _handlers;

        IHandler *getHandler(Type type) const;

        void registerHandlers();
        void registerHandler(std::unique_ptr<IHandler> handler);
    };

}
