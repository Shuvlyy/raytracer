#pragma once

#include "Exception/IException.hpp"

#include <iostream>

namespace raytracer::exception
{

    class AException
        : public IException
    {
    public:
        explicit AException(std::string message);

        [[nodiscard]] const char *what()
            const noexcept override;

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
