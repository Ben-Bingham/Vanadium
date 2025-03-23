#pragma once

#include "GenerateGeometry.h"
#include "GenerateGrid.h"

namespace Vanadium {
	struct Chunk {
		Geometry geometry{ };

		glm::ivec3 position{ };

		Grid grid{ };
	};

	class GlChunk {
	public:
		GlChunk() = default;
		GlChunk(const GlChunk& other) = delete;
		GlChunk(GlChunk&& other) noexcept = default;
		GlChunk& operator=(const GlChunk& other) = delete;
		GlChunk& operator=(GlChunk&& other) noexcept = default;
		~GlChunk() = default;

		std::unique_ptr<VertexAttributeObject> vao{ };
		std::unique_ptr<VertexBufferObject> vbo{ };
		std::unique_ptr<ElementBufferObject> ebo{ };
	};
}