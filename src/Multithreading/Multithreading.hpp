#pragma once

#include "Renderer.hpp"

#define CPU_INFO_FILE "/sys/devices/system/cpu/online"

namespace raytracer::multithreading {

    void render(Renderer &renderer);

}