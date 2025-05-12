#pragma once

#include "Network/Server/Socket/Socket.hpp"

#include "Network/Server/Game/Settings.hpp"

namespace raytracer::server
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
        [[nodiscard]] Socket &getControlSocket() { return this->_controlSocket; }
        [[nodiscard]] uint64_t getLatency() const { return this->_latency; }

        void setLatency(const uint64_t latency) { this->_latency = latency; }

    private:
        uint32_t _id;
        Socket _controlSocket;
        uint64_t _latency;
    };

}
