#include "Exception/AException.hpp"

namespace raytracer::exception
{

    AException::AException
    (
        std::string message
    )
        : _message(std::move(message))
    {}

    const char *
    AException::what
    ()
        const noexcept
    {
        return this->_message.c_str();
    }

}
