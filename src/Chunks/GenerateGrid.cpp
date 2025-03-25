#include "GenerateGrid.h"

namespace Vanadium {
	Grid CreateGrid(const ChunkPosition& cPos, int n, const Settings& settings) {
        Grid grid{ };

        grid.resize(n);
        for (auto& r : grid) {
            r.resize(n);
            for (auto& c : r) {
                c.resize(n);
            }
        }

        for (int x = 0; x < n; ++x) {
            for (int z = 0; z < n; ++z) {
                double noise = settings.noise.perlin.octave2D_01(
                    (double)x + ((double)cPos.x * (double)n) * (double)settings.noise.xMult,
                    (double)z + ((double)cPos.z * (double)n) * (double)settings.noise.zMult,
                    settings.noise.octaves
                );

                noise *= (double)settings.noise.amplitude;
                noise += (double)settings.noise.mean;

                int maxHeight = (int)std::floor(noise);

                maxHeight -= (cPos.y * n);

                for (int y = 0; y < n; ++y) {
                    //if ((cPos.x + cPos.y + cPos.z) % 2 == 0) {
                    //    grid[x][y][z] = 1;
                    //}
                    //else {
                    //    grid[x][y][z] = 2;
                    //}
                    
                    if (y > maxHeight) {
                        grid[x][y][z] = 0;

                        break;
                    }

                    if (y == maxHeight) {
                        grid[x][y][z] = 1;
                        continue;
                    }

                    if (y >= maxHeight - 3) {
                        grid[x][y][z] = 2;
                        continue;
                    }

                    grid[x][y][z] = 3;
                }
            }
        }

        return grid;
	}
}