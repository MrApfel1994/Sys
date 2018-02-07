#pragma once

#include <cstdint>

namespace sys {
unsigned int GetTicks();
uint64_t GetTimeNs();
extern unsigned int cached_time;
}
