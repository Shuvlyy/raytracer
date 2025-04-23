#pragma once

#include "Exception/IException.hpp"

#include <iostream>

namespace logger::exception
{

    class AException
        : public IException
    {
    public:
        explicit AException(std::string message)
            : _message(std::move(message)) {}

        [[nodiscard]] const char *what() const noexcept override
            { return this->_message.c_str(); }

    private:
        std::string _message;
    };

    inline std::ostream &operator<<(
        std::ostream &os,
        const IException &exception
    )
    {
        os << exception.what() << std::endl;
        return os;
    }

}
