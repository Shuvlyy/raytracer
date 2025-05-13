#pragma once

#include "Network/Socket/Socket.hpp"

namespace raytracer::network::server
{

    class Session final
    {
    public:
        explicit Session(
            int fd,
            uint32_t id
        );

        void refreshLatency();

        [[nodiscard]] uint32_t getId() const { return this->_id; }
        [[nodiscard]] Socket& getControlSocket() { return this->_controlSocket; }
        [[nodiscard]] uint64_t getLatency() const { return this->_latency; }

        void setLatency(const uint64_t latency) { this->_latency = latency; }

    private:
        uint32_t _id;
        Socket _controlSocket;
        uint64_t _latency;
    };

}
