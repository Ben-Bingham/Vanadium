#pragma once

#include "GenerateGrid.h"

namespace Vanadium {
	Grid CleanGrid(
		const Grid& grid, 
		int n,
		const Grid& pxGrid,
		const Grid& pyGrid,
		const Grid& pzGrid,
		const Grid& nxGrid,
		const Grid& nyGrid,
		const Grid& nzGrid
	);
}