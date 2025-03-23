#pragma once

#include <vector>

#include "Settings.h"

#include "Chunk.h"

namespace Vanadium {
	Grid CreateGrid(const ChunkPosition& position, int n, const Settings& settings);
}