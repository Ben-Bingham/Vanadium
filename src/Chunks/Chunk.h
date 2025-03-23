#pragma once

#include "Geometry.h"
#include "Grid.h"

#include "Utilities/OpenGl/VertexAttributeObject.h"
#include "Utilities/OpenGl/Buffer.h"

namespace Vanadium {
	using ChunkPosition = glm::ivec3;

	struct Chunk {
		Geometry geometry{ };

		ChunkPosition position{ };

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