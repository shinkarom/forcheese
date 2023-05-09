#pragma once

#include <cstdint>

using CellType = uint64_t;
constexpr auto CellSize = sizeof(CellType);
constexpr auto HeapSize = 2 * 1024 * 1024;
constexpr auto MinStackSize = 1024;