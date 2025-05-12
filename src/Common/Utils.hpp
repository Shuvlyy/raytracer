#pragma once

#include <chrono>
#include <arpa/inet.h>

namespace raytracer::utils
{

    inline uint64_t getCurrentTimestamp()
    {
        using namespace std::chrono;

        return duration_cast<milliseconds>(
            system_clock::now().time_since_epoch()
        ).count();
    }

    inline uint64_t ntohll(uint64_t value)
    {
#if __BYTE_ORDER == __LITTLE_ENDIAN
        return (static_cast<uint64_t>(ntohl(static_cast<uint32_t>(value >> 32))) |
               (static_cast<uint64_t>(ntohl(static_cast<uint32_t>(value & 0xFFFFFFFF))) << 32));
#else
        return value;
#endif
    }

    inline uint64_t htonll(uint64_t value)
    {
#if __BYTE_ORDER == __LITTLE_ENDIAN
        return (static_cast<uint64_t>(htonl(static_cast<uint32_t>(value >> 32))) |
               (static_cast<uint64_t>(htonl(static_cast<uint32_t>(value & 0xFFFFFFFF))) << 32));
#else
        return value;
#endif
    }

}
