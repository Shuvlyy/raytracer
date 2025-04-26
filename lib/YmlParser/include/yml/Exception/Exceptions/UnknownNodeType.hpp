#pragma once

#include "yml/Exception/AException.hpp"

namespace yml::exception
{

    class UnknownNodeType final
        : public AException
    {
    public:
        explicit UnknownNodeType(
            const std::string& name
        )
            : AException(std::string(name) + ": Unknown node type.")
        {}
    };

}
