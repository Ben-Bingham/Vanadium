#pragma once

#include <glm/glm.hpp>

#include "Grid.h"
#include "Chunk.h"
#include "Geometry.h"

namespace Vanadium {
	Geometry GenerateGeometry(
		const ChunkPosition& position, 
		const Grid& grid, 
		int n,
		const Grid& pxGrid,
		const Grid& pyGrid,
		const Grid& pzGrid,
		const Grid& nxGrid,
		const Grid& nyGrid,
		const Grid& nzGrid
	);

	std::vector<float> VerticesAsFloatVector(const std::vector<Vertex>& vertices);
}