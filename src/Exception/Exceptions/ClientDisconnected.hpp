#pragma once

#include "Exception/AException.hpp"

#include <format>

namespace raytracer::exception
{

    class ClientDisconnected final
        : public AException
    {
    public:
        explicit ClientDisconnected(const int fd)
            : AException(std::format("{}: Client disconnected.", fd))
        {}
    };

}
