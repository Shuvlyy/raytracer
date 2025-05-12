#pragma once

#include <mutex>
#include <queue>

#include "Network/Packet/Packet.hpp"

namespace raytracer::network
{

    class Channel final {
        public:
            explicit Channel() = default;
            ~Channel() = default;

            Channel(const Channel&) = delete;
            Channel& operator=(const Channel&) = delete;

            void push(std::unique_ptr<Packet> packet);
            [[nodiscard]] std::unique_ptr<Packet> pop();

            [[nodiscard]] size_t size() const {
                std::lock_guard lock(this->_mutex);
                return this->_queue.size();
            }
            [[nodiscard]] size_t empty() const {
                std::lock_guard lock(this->_mutex);
                return this->_queue.empty();
            }

        private:
            std::queue<std::unique_ptr<Packet>> _queue;
            mutable std::mutex _mutex;
    };

}
