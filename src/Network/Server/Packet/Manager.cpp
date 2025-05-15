#include "Manager.hpp"

#include "logger/Logger.hpp"

#include "Handlers/Pong.hpp"
#include "Handlers/Cestciao.hpp"
#include "Handlers/Finito.hpp"

namespace raytracer::network::packet::server
{

    Manager::Manager
    (
        Server& server
    )
        : _server(server)
    {
        this->registerHandlers();
    }

    void
    Manager::dispatchPacket
    (
        const Packet& packet,
        network::server::Session& session
    )
        const
    {
        LOG_DEBUG(
            "Received Packet " + Packet::fromTypeToString(packet.getType()) +
            " (SID: " + std::to_string(session.getId()) + ")"
        );

        const IHandler* handler = this->getHandler(packet.getType());

        if (!handler) {
            LOG_DEBUG("No handler has been registered for this kind of Packet. Ignoring.");
            return;
        }

        handler->handle(packet, this->_server, session);
    }

    void
    Manager::registerHandlers
    ()
    {
        this->registerHandler(std::make_unique<handler::Pong>());
        this->registerHandler(std::make_unique<handler::Cestciao>());
        this->registerHandler(std::make_unique<handler::Finito>());
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
            "Registered Packet handler " +
            Packet::fromTypeToString(trigger)
        );
    }

    IHandler*
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
