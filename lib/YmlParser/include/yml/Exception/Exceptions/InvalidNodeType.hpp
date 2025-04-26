#pragma once

#include "yml/Exception/AException.hpp"

namespace yml::exception
{

    class InvalidNodeType final
        : public AException
    {
    public:
        explicit InvalidNodeType(
            const std::string& name,
            const std::string& type
        )
            : AException(std::string(name) + ": Invalid node type: " + type + ".")
        {}
    };

}
