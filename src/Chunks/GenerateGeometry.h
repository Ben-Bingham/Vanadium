#pragma once

#include <glm/glm.hpp>

#include "Grid.h"
#include "Chunk.h"
#include "Geometry.h"

namespace Vanadium {
	Geometry GenerateGeometry(const ChunkPosition& position, const Grid& grid, int n, int atlasWidth, int atlasHeight);

	std::vector<float> VerticesAsFloatVector(const std::vector<Vertex>& vertices);
}