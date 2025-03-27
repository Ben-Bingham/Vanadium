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
                    grid[x][y][z] = GenerateBlock(glm::ivec3{ x, y, z } + (cPos * n), cPos, n, (float)maxHeight);
                }
            }
        }

        return grid;
	}

    Grid CreatePartialGrid( // TODO
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
                    grid[x][y][z] = GenerateBlock(glm::ivec3{ x, y, z } + (cPos * n), cPos, n, (float)maxHeight);
                }
            }
        }

        return grid;
    }

    BlockIndex GenerateBlock(const glm::ivec3& pos, const ChunkPosition& cPos, int n, float noise) {
        //if (pos.y >= 5) {
        //    return 0;
        //}
        //
        //glm::ivec3 lPos = pos - (cPos * n);
        //
        //if (lPos.x == lPos.z && lPos.x == 0) {
        //    return 0;
        //}
        //
        //if (lPos.x == lPos.z && lPos.x == 1) {
        //    return 0;
        //}
        //
        //return 1;


        if (pos.y > noise) {
            return 0;
        }

        if (pos.y == noise) {
            return 1;
        }

        if (pos.y >= noise - 3) {
            return 2;
        }

        return 3;
    }
}