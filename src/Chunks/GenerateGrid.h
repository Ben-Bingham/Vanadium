#pragma once

#include <vector>

#include "Settings.h"

#include "Chunk.h"

namespace Vanadium {
	Grid CreateGrid(const ChunkPosition& cPos, int n, const Settings& settings);

	Grid CreatePartialGrid(
		const ChunkPosition& cPos,
		int n,
		const Settings& settings,
		const glm::ivec3& bottom,
		const glm::ivec3& top
	);

	BlockIndex GenerateBlock(const glm::ivec3& pos, float noise);
}