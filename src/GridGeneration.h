#pragma once

#include <vector>

#include "Settings.h"

namespace Vanadium {
	using BlockIndex = unsigned char;

	using Grid = std::vector<std::vector<std::vector<BlockIndex>>>;

	Grid CreateGrid(int n, const Settings& settings);
}