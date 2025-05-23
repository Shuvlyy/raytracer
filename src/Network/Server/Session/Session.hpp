#pragma once

#define TIME_POINT std::chrono::steady_clock::time_point

#include "Network/Socket/Socket.hpp"
#include "Renderer/Tile.hpp"

#include <chrono>

namespace raytracer::network::server
{

    namespace session
    {

        enum class State
        {
            READY,
            RENDERING,
            DEADASS
        };

        struct Data
        {
            renderer::Tile currentTile;
        };

    }

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
        [[nodiscard]] TIME_POINT getLastLatencyRefresh() const
            { return this->_lastLatencyRefresh; }
        [[nodiscard]] session::State getState() const { return _state; }
        [[nodiscard]] session::Data& getData() { return this->_data; }

        void setLatency(const uint64_t latency) { this->_latency = latency; }
        void setLastLatencyRefresh(const TIME_POINT lastLatencyRefresh)
            { this->_lastLatencyRefresh = lastLatencyRefresh; }
        void setState(const session::State state) { this->_state = state; }

    private:
        uint32_t _id;
        Socket _controlSocket;
        uint64_t _latency;
        TIME_POINT _lastLatencyRefresh;
        session::State _state;
        session::Data _data;
    };

}
