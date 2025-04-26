#pragma once

#include "yml/Exception/IException.hpp"

#include <string>
#include <utility>

namespace yml::exception
{

    class AException
        : public IException
    {
    public:
        explicit AException(std::string message) :
            _message(std::move(message)) {}

        [[nodiscard]] const char *what() const noexcept override
            { return this->_message.c_str(); }

    private:
        std::string _message;
    };

}
