#include "CleanGrid.h"

namespace Vanadium {
	Grid CleanGrid(
        const Grid& grid,
        int n,
        const Grid& pxGrid,
        const Grid& pyGrid,
        const Grid& pzGrid,
        const Grid& nxGrid,
        const Grid& nyGrid,
        const Grid& nzGrid) {

        Grid cleanGrid = grid;

        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                for (int z = 0; z < n; ++z) {
                    if (cleanGrid[x][y][z] == 0) continue;

                    Vanadium::BlockIndex px = 0, py = 0, pz = 0, nx = 0, ny = 0, nz = 0;

                    if (x + 1 < n) { 
                        px = grid[x + 1][y][z]; 
                    }
                    else {
                        px = pxGrid[0][y][z];
                    }

                    if (y + 1 < n) { 
                        py = grid[x][y + 1][z]; 
                    }
                    else {
                        py = pyGrid[x][0][z];
                    }

                    if (z + 1 < n) { 
                        pz = grid[x][y][z + 1]; 
                    }
                    else {
                        pz = pzGrid[x][y][0];
                    }

                    if (x - 1 >= 0) { 
                        nx = grid[x - 1][y][z]; 
                    }
                    else {
                        nx = nxGrid[n - 1][y][z];
                    }

                    if (y - 1 >= 0) { 
                        ny = grid[x][y - 1][z]; 
                    }
                    else {
                        ny = nyGrid[x][n - 1][z];
                    }

                    if (z - 1 >= 0) { 
                        nz = grid[x][y][z - 1]; 
                    }
                    else {
                        nz = nzGrid[x][y][n - 1];
                    }

                    if (px && py && pz && nx && ny && nz) {
                        cleanGrid[x][y][z] = 0;
                    }
                }
            }
        }

        return cleanGrid;
	}
}