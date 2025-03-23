#pragma once

#include <vector>

namespace Vanadium {
	using BlockIndex = unsigned char;

	using Grid = std::vector<std::vector<std::vector<BlockIndex>>>;
}