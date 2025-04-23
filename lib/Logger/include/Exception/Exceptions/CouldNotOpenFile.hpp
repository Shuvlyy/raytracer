#pragma once

#include "Exception/AException.hpp"

#include <string>

namespace logger::exception
{

    class CouldNotOpenFile final
        : public AException
    {
    public:
        explicit CouldNotOpenFile(const std::string &filepath)
            : AException("Could not open file: \"" + filepath + "\"")
        {}
    };

}
