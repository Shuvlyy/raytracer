#include "Manager.hpp"
#include "App.hpp"

#include "Handlers/Term.hpp"
#include "Handlers/Int.hpp"

#include "logger/Logger.hpp"

#include <csignal>

namespace raytracer::signal
{

    Manager *Manager::instance = nullptr;

    Manager::Manager
    (
        App *app
    )
        : _app(app)
    {
        instance = this;

        this->registerHandlers();
        this->setupListener();
    }

    void
    Manager::registerHandlers()
    {
        this->registerHandler(std::make_unique<handler::Term>());
        this->registerHandler(std::make_unique<handler::Int>());
    }

    void
    Manager::registerHandler
    (
        std::unique_ptr<IHandler> handler
    )
    {
        this->_handlers.insert(std::make_pair(
            handler->getSignalNumber(),
            std::move(handler)
        ));
    }

    void
    Manager::setupListener
    ()
    {
        for (const auto &[signalNumber, handler] : this->_handlers) {
            std::signal(
                signalNumber,
                [](const int signum) { dispatchHandler(signum); }
            );
        }
    }

    void
    Manager::dispatchHandler
    (
        const int signum
    )
    {
        const auto it = instance->_handlers.find(signum);

        if (it != instance->_handlers.end()) {
            const std::unique_ptr<IHandler> &handler = it->second;

            LOG_DEBUG("Caught signal " + handler->getSignalName());
            handler->handle(instance->_app);
        }
    }

}
