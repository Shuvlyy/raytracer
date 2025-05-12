#include "Channel.hpp"

namespace raytracer::network
{

    std::unique_ptr<Packet>
    Channel::pop
    ()
    {
        std::lock_guard lock(this->_mutex);

        if (this->_queue.empty())
            return nullptr;
        std::unique_ptr<Packet> packet = std::move(this->_queue.front());
        this->_queue.pop();
        return packet;
    }

    void
    Channel::push(std::unique_ptr<Packet> packet)
    {
        std::lock_guard lock(this->_mutex);
        this->_queue.push(std::move(packet));
    }

}