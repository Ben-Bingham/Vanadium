#include "GenerateChunk.h"

#include "CleanGrid.h"
#include "GenerateGeometry.h"

namespace Vanadium {
	Chunk GenerateChunk(const ChunkPosition& position, const Settings& settings, int n, int atlasWidth, int atlasHeight) {
		Chunk chunk{ };

		chunk.position = position;

		chunk.grid = CreateGrid(position, n, settings);
		chunk.grid = CleanGrid(chunk.grid, n);
		chunk.geometry = GenerateGeometry(position, chunk.grid, n, atlasWidth, atlasHeight);

		return chunk;
	}
}