#pragma once

#include "Chunks/Chunk.h"

namespace Vanadium {
	struct Job {
		ChunkPosition position{ };

		size_t priority{ 0 }; // Higher is more urgent
	};

	class JobSystem {
	public:

	private:

	};
}