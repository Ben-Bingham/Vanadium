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
                    //    grid[x][y][z] = 0;
                    //}
                    //else {
                    //    grid[x][y][z] = 2;
                    //}
                    //continue;
                    // 
                    //if (x == 0 == z) {
                    //    grid[x][y][z] = 1;
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

    Grid CreatePartialGrid(
        const ChunkPosition& cPos,
        int n,
        const Settings& settings,
        const glm::ivec3& bottom,
        const glm::ivec3& top
    ) {
        Grid grid{ };

        grid.resize(top.x - bottom.x);
        for (auto& r : grid) {
            r.resize(top.y - bottom.y);
            for (auto& c : r) {
                c.resize(top.z - bottom.z);
            }
        }

        for (int x = bottom.x; x < top.x; ++x) {
            for (int z = bottom.z; z < top.z; ++z) {
                double noise = settings.noise.perlin.octave2D_01(
                    (double)x + ((double)cPos.x * (double)n) * (double)settings.noise.xMult,
                    (double)z + ((double)cPos.z * (double)n) * (double)settings.noise.zMult,
                    settings.noise.octaves
                );

                noise *= (double)settings.noise.amplitude;
                noise += (double)settings.noise.mean;

                int maxHeight = (int)std::floor(noise);

                maxHeight -= (cPos.y * n);

                for (int y = bottom.y; y < top.y; ++y) {
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