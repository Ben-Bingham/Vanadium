#pragma once

#include "Chunk.h"
#include "Settings.h"

namespace Vanadium {
	Chunk GenerateChunk(const ChunkPosition& position, const Settings& settings, int n, int atlasWidth, int atlasHeight);
}