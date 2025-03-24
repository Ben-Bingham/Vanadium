#pragma once

#include "Geometry.h"
#include "Grid.h"

#include "Utilities/OpenGl/VertexAttributeObject.h"
#include "Utilities/OpenGl/Buffer.h"

namespace Vanadium {
	using ChunkPosition = glm::ivec3;

	struct Chunk {
		ChunkPosition position{ };

		Grid grid{ };

		bool remakeChunk{ true };

		Geometry geometry{ };

		// TODO seperate vao's for each face set
		std::unique_ptr<VertexAttributeObject> vao{ };
		std::unique_ptr<VertexBufferObject> vbo{ };
		std::unique_ptr<ElementBufferObject> ebo{ };
	};
}