#include "Network/Client/Packet/IHandler.hpp"
#include "Exception/Exceptions/ServerDisconnected.hpp"

#include "Client.hpp"

#include <poll.h>
#include <thread>

#include "Exception/Exceptions/ClientDisconnected.hpp"

namespace raytracer::app
{

    Client::Client
    (
        const Attributes& attributes
    )
        : App(attributes),
          _client(attributes.host, attributes.port)
    {}

    void
    Client::run()
    {
        LOG_INFO("Client running...");
        this->_running = true;
        auto exFuture = this->_exceptionPromise.get_future();

        std::thread th(&network::Client::run, &this->_client, std::ref(*this));
        while (this->_running.load()) {
            if (exFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
                try {
                    if (std::exception_ptr eptr = exFuture.get()) {
                        std::rethrow_exception(eptr);
                    }
                } catch (const exception::ServerDisconnected &e) {
                    LOG_FATAL("Child thread exception: " + std::string(e.what()));
                } catch (const std::exception &e) {
                    LOG_ERR("Child thread error: " + std::string(e.what()));
                }
                this->stop();
                break;
            }
            if (this->_client.hasPacketToProcess()) {
                auto packet = this->_client.popPacket();
                if (packet != nullptr) {
                    this->_manager.dispatchPacket(*packet, *this);
                }
            }
        }
    }

    void
    Client::stop()
    {
        LOG_INFO("Client stopping...");
        this->_shouldStop = true;
        this->_running = false;
        this->_client.stop();
    }

}
