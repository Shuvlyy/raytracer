#pragma once

#include "logger/Logger.hpp"

#include <chrono>
#include <arpa/inet.h>
#include <filesystem>

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

    inline uint16_t getNbProcs()
    {
        const int defaultCpuAmount = 1;
        const std::string cpuInfoFilepath = "/sys/devices/system/cpu/online";

        if (!std::filesystem::exists(cpuInfoFilepath)) {
            return defaultCpuAmount;
        }

        std::ifstream file(cpuInfoFilepath);
        if (!file.is_open()) {
            LOG_WARN("Couldn't open cpu file info, disabling multithreading");
            return defaultCpuAmount;
        }

        std::filebuf *s = file.rdbuf();
        std::stringstream buffer;
        buffer << s;
        std::string temp = buffer.str();
        file.close();

        return std::stoi(temp.substr(2)) + 1;
    }

}
