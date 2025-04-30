#pragma once

#include "Exception/IException.hpp"

#include <iostream>
#include <utility>

namespace raytracer::exception
{

    class AException
        : public IException
    {
    public:
        explicit AException(
            std::string message
        ) : _message(std::move(message)) {}

        [[nodiscard]] const char *what() const noexcept override
            { return this->_message.c_str(); }

    private:
        std::string _message;
    };

}
