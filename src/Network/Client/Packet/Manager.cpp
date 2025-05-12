#include "Network/Client/Packet/Handlers/Pong.hpp"
#include "Network/Client/Packet/Handlers/Ping.hpp"

#include "Network/Packet/Packet.hpp"

#include "Manager.hpp"
#include "Logger.hpp"

namespace raytracer::network::packet
{

    Manager::Manager
    ()
    {
        this->registerHandlers();
    }

    void
    Manager::dispatchPacket
    (
        const Packet &packet
    )
        const
    {
        LOG_DEBUG("Received Packet " + Packet::fromTypeToString(packet.getType()) + "`");

        const IHandler *handler = this->getHandler(packet.getType());

        if (!handler) {
            LOG_DEBUG("No handler has been registered for this kind of Packet. Ignoring.");
            return; // TODO: Throw exception?
        }

        handler->handle(packet);
    }

    void
    Manager::registerHandlers
    ()
    {
        this->registerHandler(std::make_unique<handler::Ping>());
        this->registerHandler(std::make_unique<handler::Pong>());
    }

    void
    Manager::registerHandler
    (
        std::unique_ptr<IHandler> handler
    )
    {
        const Type trigger = handler->getTrigger();

        this->_handlers.insert({
            trigger,
            std::move(handler)
        });

        LOG_DEBUG(
            "(PacketManager) Registered Packet handler " +
            Packet::fromTypeToString(trigger)
        );
    }

    IHandler *
    Manager::getHandler
    (
        const Type type
    )
        const
    {
        if (!this->_handlers.contains(type)) {
            return nullptr;
        }

        return this->_handlers.at(type).get();
    }

}
