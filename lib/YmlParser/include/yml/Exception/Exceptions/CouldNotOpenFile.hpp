#pragma once

#include "yml/Exception/AException.hpp"

namespace yml::exception
{

    class CouldNotOpenFile final
        : public AException
    {
    public:
        explicit CouldNotOpenFile(const std::string& error)
            : AException("Could not open file: " + error)
        {}
    };

}
