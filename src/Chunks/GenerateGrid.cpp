#include "GenerateGrid.h"

namespace Vanadium {
	Grid CreateGrid(const ChunkPosition& position, int n, const Settings& settings) {
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
                double noise = settings.noise.perlin.octave2D_01((double)(x + (position.x * n)) * (double)settings.noise.xMult, (double)(z + (position.z * n)) * (double)settings.noise.yMult, settings.noise.octaves);
                noise *= settings.noise.percentOfBlocksAffected;
                noise += 1.0;
                noise -= settings.noise.percentOfBlocksAffected;

                noise *= settings.noise.noiseMult;
                noise += settings.noise.noiseOffset;

                noise *= (double)n;

                for (int y = 0; y < n; ++y) {
                    if (y > noise) {
                        grid[x][y][z] = 0;

                        break;
                    }

                    if (y == noise) {
                        grid[x][y][z] = 1;
                        continue;
                    }

                    if (y >= noise - 3) {
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