#pragma once

#include <memory>
#include <unordered_map>

namespace raytracer::app {

    class Client;

}

namespace raytracer::network::packet::client
{
    class IHandler;

    class Manager final
    {
    public:
        explicit Manager();

        void dispatchPacket(
            const Packet &packet,
            app::Client &cli
        ) const;

    private:
        std::unordered_map<Type, std::unique_ptr<IHandler>> _handlers;

        IHandler *getHandler(Type type) const;

        void registerHandlers();
        void registerHandler(std::unique_ptr<IHandler> handler);
    };

}
