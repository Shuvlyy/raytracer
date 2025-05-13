#pragma once

#define TIME_POINT std::chrono::steady_clock::time_point

#include <chrono>

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
        [[nodiscard]] TIME_POINT getLastLatencyRefresh() const { return this->_lastLatencyRefresh; }

        void setLatency(const uint64_t latency) { this->_latency = latency; }
        void setLastLatencyRefresh(const TIME_POINT lastLatencyRefresh) { this->_lastLatencyRefresh = lastLatencyRefresh; }

    private:
        uint32_t _id;
        Socket _controlSocket;
        uint64_t _latency;
        TIME_POINT _lastLatencyRefresh;
    };

}
