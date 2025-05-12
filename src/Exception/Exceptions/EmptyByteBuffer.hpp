#pragma once

#include "Exception/AException.hpp"

namespace raytracer::exception
{

    class EmptyByteBuffer final
        : public AException
    {
    public:
        explicit EmptyByteBuffer()
            : AException("Got an empty ByteBuffer")
        {}
    };

}
