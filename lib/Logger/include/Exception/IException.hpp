#pragma once

#include <exception>

namespace logger::exception
{

    class IException
        : public std::exception
    {
    public:
        [[nodiscard]] const char *what() const noexcept override = 0;
    };

}
