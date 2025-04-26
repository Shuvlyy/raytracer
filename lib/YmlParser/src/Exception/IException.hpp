#pragma once

#include <exception>

namespace yml::exception
{

    class IException
        : public std::exception
    {
    public:
        [[nodiscard]] const char *what() const noexcept override = 0;
    };

}
