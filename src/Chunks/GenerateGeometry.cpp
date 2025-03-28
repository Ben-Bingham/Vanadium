#include "GenerateGeometry.h"

#include <array>

#include <glm/gtc/matrix_transform.hpp>

namespace Vanadium {
    std::vector<Vertex> pxVerts{
        Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f, 0.0f }, 0.0f },
        Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0f },
        Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f, 1.0f }, 0.0f },
        Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f, 0.0f }, 0.0f },
    };

    std::vector<Vertex> pyVerts{
        Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 0.0f, 1.0f }, 0.0f },
        Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0f },
        Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 1.0f, 0.0f }, 0.0f },
        Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 0.0f, 0.0f }, 0.0f }
    };

    std::vector<Vertex> pzVerts{
        Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0f },
        Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 1.0f, 0.0f }, 0.0f },
        Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 0.0f, 0.0f }, 0.0f },
        Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 0.0f, 1.0f }, 0.0f },
    };

    std::vector<Vertex> nxVerts{
        Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{-1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f, 1.0f }, 0.0f },
        Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{-1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0f },
        Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f }, glm::vec3{-1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f, 0.0f }, 0.0f },
        Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{-1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f, 0.0f }, 0.0f },
    };

    std::vector<Vertex> nyVerts{
        Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 1.0f, 0.0f }, 0.0f },
        Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0f },
        Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 0.0f, 1.0f }, 0.0f },
        Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 0.0f, 0.0f }, 0.0f },
    };

    std::vector<Vertex> nzVerts{
        Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 0.0f, 0.0f }, 0.0f },
        Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 1.0f, 0.0f }, 0.0f },
        Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 1.0f, 1.0f }, 0.0f },
        Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 0.0f, 1.0f }, 0.0f },
    };

    std::vector<Index> faceIndices{
        2,  1,  0,
        0,  3,  2,
    };

    struct StartEndPair {
        glm::ivec2 start;
        glm::ivec2 end;

        BlockIndex blockIndex;
    };

    std::vector<StartEndPair> GetGreedyFaces(std::vector<std::vector<BlockIndex>> mask, int n) {
        std::vector<StartEndPair> sePairs;
        std::vector<BlockIndex> differentBlocks{ };

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (mask[i][j] == 0) continue;

                if (std::find(differentBlocks.begin(), differentBlocks.end(), mask[i][j]) == differentBlocks.end()) {
                    differentBlocks.push_back(mask[i][j]);
                }
            }
        }

        for (BlockIndex& blockIndex : differentBlocks) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (mask[i][j] != blockIndex) continue;

                    glm::ivec2 start{ i, j };
                    glm::ivec2 end{ start };

                    for (int p = 0; start.x + p < n; ++p) {
                        if (mask[start.x + p][start.y] == blockIndex) {
                            end.x = start.x + p;
                        }
                        else {
                            break;
                        }
                    }

                    for (int p = 0; start.y + p < n; ++p) {
                        bool cond = true;
                        for (int q = start.x; q < end.x + 1; ++q) {
                            cond = cond && start.x + q < n && mask[start.x + q][start.y + p] == blockIndex;
                        }

                        if (cond) {
                            end.y = start.y + p;
                        }
                        else {
                            break;
                        }
                    }

                    sePairs.push_back(StartEndPair{ start, end, blockIndex });

                    for (int p = start.x; p < end.x + 1; ++p) {
                        for (int q = start.y; q < end.y + 1; ++q) {
                            mask[p][q] = 0;
                        }
                    }
                }
            }
        }

        return sePairs;
    }

	Geometry GenerateGeometry(
        const ChunkPosition& position,
        const Grid& grid,
        int n,
        const Grid& pxGrid,
        const Grid& pyGrid,
        const Grid& pzGrid,
        const Grid& nxGrid,
        const Grid& nyGrid,
        const Grid& nzGrid) {
        Geometry geo{ };

        std::vector<std::vector<std::vector<std::array<BlockIndex, 6>>>> faceStatus{ };

        faceStatus.resize(n);
        for (auto& r : faceStatus) {
            r.resize(n);
            for (auto& c : r) {
                c.resize(n);
                for (auto& p : c) {
                    for (int i = 0; i < 6; ++i) {
                        p[i] = (BlockIndex)0; // Default values TODO got to be a better way to do this
                    }
                }
            }
        }

        // faces:
        // 0 -> px
        // 1 -> py
        // 2 -> pz
        // 3 -> nx
        // 4 -> ny
        // 5 -> nz

        for (int x = 0; x < n; ++x) {
            for (int y = 0; y < n; ++y) {
                for (int z = 0; z < n; ++z) {
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

                    if (px == 0) {
                        faceStatus[x][y][z][0] = grid[x][y][z];
                    }

                    if (py == 0) {
                        faceStatus[x][y][z][1] = grid[x][y][z];
                    }

                    if (pz == 0) {
                        faceStatus[x][y][z][2] = grid[x][y][z];
                    }

                    if (nx == 0) {
                        faceStatus[x][y][z][3] = grid[x][y][z];
                    }

                    if (ny == 0) {
                        faceStatus[x][y][z][4] = grid[x][y][z];
                    }

                    if (nz == 0) {
                        faceStatus[x][y][z][5] = grid[x][y][z];
                    }
                }
            }
        }

        // Positive x faces
        for (int x = 0; x < n; ++x) {
            std::vector<std::vector<BlockIndex>> mask{ };

            mask.resize(n);
            for (auto& r : mask) {
                r.resize(n);
            }

            for (int y = 0; y < n; ++y) {
                for (int z = 0; z < n; ++z) {
                    if (faceStatus[x][y][z][0] == 0) continue;

                    mask[y][z] = faceStatus[x][y][z][0];
                }
            }

            std::vector<StartEndPair> sePairs = GetGreedyFaces(mask, n);

            for (auto& pair : sePairs) {
                glm::ivec3 globalStart = glm::ivec3{ x, pair.start.x, pair.start.y } + (position * n);
                glm::ivec3 globalEnd = glm::ivec3{ x, pair.end.x, pair.end.y } + (position * n);

                glm::ivec2 texRepeatCount = pair.end - pair.start;
                texRepeatCount += 1;

                std::vector<Vertex> verts{
                    Vertex{ glm::vec3{ (float)x + (glm::vec3{ position } * (float)n).x + 0.5f, globalEnd.y   + 0.5f, globalEnd.z   + 0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ texRepeatCount.x, 0.0f             }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ (float)x + (glm::vec3{ position } * (float)n).x + 0.5f, globalEnd.y   + 0.5f, globalStart.z - 0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ texRepeatCount.x, texRepeatCount.y }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ (float)x + (glm::vec3{ position } * (float)n).x + 0.5f, globalStart.y - 0.5f, globalStart.z - 0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f,             texRepeatCount.y }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ (float)x + (glm::vec3{ position } * (float)n).x + 0.5f, globalStart.y - 0.5f, globalEnd.z + 0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f,             0.0f             }, (float)pair.blockIndex }
                };

                geo.vertices.insert(geo.vertices.end(), verts.begin(), verts.end());
            }
        }

        // Positive y faces
        for (int y = 0; y < n; ++y) {
            std::vector<std::vector<BlockIndex>> mask{ };

            mask.resize(n);
            for (auto& r : mask) {
                r.resize(n);
            }

            for (int x = 0; x < n; ++x) {
                for (int z = 0; z < n; ++z) {
                    if (faceStatus[x][y][z][1] == 0) continue;

                    mask[x][z] = faceStatus[x][y][z][1];
                }
            }

            std::vector<StartEndPair> sePairs = GetGreedyFaces(mask, n);

            for (auto& pair : sePairs) {
                glm::ivec3 globalStart = glm::ivec3{ pair.start.x, y, pair.start.y } + (position * n);
                glm::ivec3 globalEnd = glm::ivec3{ pair.end.x, y, pair.end.y } + (position * n);

                glm::ivec2 texRepeatCount = pair.end - pair.start;
                texRepeatCount += 1;

                std::vector<Vertex> verts{
                    Vertex{ glm::vec3{ globalStart.x - 0.5f, (float)y + (glm::vec3{ position } * (float)n).y + 0.5f, globalStart.z - 0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 0.0f,             texRepeatCount.y }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalEnd.x   + 0.5f, (float)y + (glm::vec3{ position } * (float)n).y + 0.5f, globalStart.z - 0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ texRepeatCount.x, texRepeatCount.y }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalEnd.x   + 0.5f, (float)y + (glm::vec3{ position } * (float)n).y + 0.5f, globalEnd.z   + 0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ texRepeatCount.x, 0.0f             }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalStart.x - 0.5f, (float)y + (glm::vec3{ position } * (float)n).y + 0.5f, globalEnd.z   + 0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 0.0f,             0.0f             }, (float)pair.blockIndex }
                };

                geo.vertices.insert(geo.vertices.end(), verts.begin(), verts.end());
            }
        }

        // Positive z faces
        for (int z = 0; z < n; ++z) {
            std::vector<std::vector<BlockIndex>> mask{ };

            mask.resize(n);
            for (auto& r : mask) {
                r.resize(n);
            }

            for (int x = 0; x < n; ++x) {
                for (int y = 0; y < n; ++y) {
                    if (faceStatus[x][y][z][2] == 0) continue;

                    mask[x][y] = faceStatus[x][y][z][2];
                }
            }

            std::vector<StartEndPair> sePairs = GetGreedyFaces(mask, n);

            for (auto& pair : sePairs) {
                glm::ivec3 globalStart = glm::ivec3{ pair.start.x, pair.start.y, z } + (position * n);
                glm::ivec3 globalEnd = glm::ivec3{ pair.end.x, pair.end.y, z } + (position * n);

                glm::ivec2 texRepeatCount = pair.end - pair.start;
                texRepeatCount += 1;

                std::vector<Vertex> verts{
                    Vertex{ glm::vec3{ globalEnd.x   + 0.5f, globalEnd.y   + 0.5f, (float)z + (glm::vec3{ position } * (float)n).z + 0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ texRepeatCount.x, texRepeatCount.y }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalEnd.x   + 0.5f, globalStart.y - 0.5f, (float)z + (glm::vec3{ position } * (float)n).z + 0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ texRepeatCount.x, 0.0f             }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalStart.x - 0.5f, globalStart.y - 0.5f, (float)z + (glm::vec3{ position } * (float)n).z + 0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 0.0,              0.0f             }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalStart.x - 0.5f, globalEnd.y   + 0.5f, (float)z + (glm::vec3{ position } * (float)n).z + 0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 0.0f,             texRepeatCount.y }, (float)pair.blockIndex }
                };

                geo.vertices.insert(geo.vertices.end(), verts.begin(), verts.end());
            }
        }

        // Negative x faces
        for (int x = 0; x < n; ++x) {
            std::vector<std::vector<BlockIndex>> mask{ };

            mask.resize(n);
            for (auto& r : mask) {
                r.resize(n);
            }

            for (int y = 0; y < n; ++y) {
                for (int z = 0; z < n; ++z) {
                    if (faceStatus[x][y][z][3] == 0) continue;

                    mask[y][z] = faceStatus[x][y][z][3];
                }
            }

            std::vector<StartEndPair> sePairs = GetGreedyFaces(mask, n);

            for (auto& pair : sePairs) {
                glm::ivec3 globalStart = glm::ivec3{ x, pair.start.x, pair.start.y } + (position * n);
                glm::ivec3 globalEnd = glm::ivec3{ x, pair.end.x, pair.end.y } + (position * n);

                glm::ivec2 texRepeatCount = pair.end - pair.start;
                texRepeatCount += 1;

                std::vector<Vertex> verts{
                    Vertex{ glm::vec3{ (float)x + (glm::vec3{ position } * (float)n).x - 0.5f, globalStart.y - 0.5f, globalStart.z - 0.5f }, glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f,             texRepeatCount.y }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ (float)x + (glm::vec3{ position } * (float)n).x - 0.5f, globalEnd.y   + 0.5f, globalStart.z - 0.5f }, glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ texRepeatCount.x, texRepeatCount.y }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ (float)x + (glm::vec3{ position } * (float)n).x - 0.5f, globalEnd.y   + 0.5f, globalEnd.z   + 0.5f }, glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ texRepeatCount.x, 0.0f             }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ (float)x + (glm::vec3{ position } * (float)n).x - 0.5f, globalStart.y - 0.5f, globalEnd.z   + 0.5f }, glm::vec3{ -1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f,             0.0f             }, (float)pair.blockIndex }
                };

                geo.vertices.insert(geo.vertices.end(), verts.begin(), verts.end());
            }
        }

        // Negative y faces
        for (int y = 0; y < n; ++y) {
            std::vector<std::vector<BlockIndex>> mask{ };

            mask.resize(n);
            for (auto& r : mask) {
                r.resize(n);
            }

            for (int x = 0; x < n; ++x) {
                for (int z = 0; z < n; ++z) {
                    if (faceStatus[x][y][z][4] == 0) continue;

                    mask[x][z] = faceStatus[x][y][z][4];
                }
            }

            std::vector<StartEndPair> sePairs = GetGreedyFaces(mask, n);

            for (auto& pair : sePairs) {
                glm::ivec3 globalStart = glm::ivec3{ pair.start.x, y, pair.start.y } + (position * n);
                glm::ivec3 globalEnd = glm::ivec3{ pair.end.x, y, pair.end.y } + (position * n);

                glm::ivec2 texRepeatCount = pair.end - pair.start;
                texRepeatCount += 1;

                std::vector<Vertex> verts{
                    Vertex{ glm::vec3{ globalEnd.x   + 0.5f, (float)y + (glm::vec3{ position } * (float)n).y - 0.5f, globalEnd.z   + 0.5f }, glm::vec3{ 0.0f,  -1.0f,  0.0f }, glm::vec2{ texRepeatCount.x, 0.0f             }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalEnd.x   + 0.5f, (float)y + (glm::vec3{ position } * (float)n).y - 0.5f, globalStart.z - 0.5f }, glm::vec3{ 0.0f,  -1.0f,  0.0f }, glm::vec2{ texRepeatCount.x, texRepeatCount.y }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalStart.x - 0.5f, (float)y + (glm::vec3{ position } * (float)n).y - 0.5f, globalStart.z - 0.5f }, glm::vec3{ 0.0f,  -1.0f,  0.0f }, glm::vec2{ 0.0f,             texRepeatCount.y }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalStart.x - 0.5f, (float)y + (glm::vec3{ position } * (float)n).y - 0.5f, globalEnd.z   + 0.5f }, glm::vec3{ 0.0f,  -1.0f,  0.0f }, glm::vec2{ 0.0f,             0.0f             }, (float)pair.blockIndex }
                };

                geo.vertices.insert(geo.vertices.end(), verts.begin(), verts.end());
            }
        }

        // Negative z faces
        for (int z = 0; z < n; ++z) {
            std::vector<std::vector<BlockIndex>> mask{ };

            mask.resize(n);
            for (auto& r : mask) {
                r.resize(n);
            }

            for (int x = 0; x < n; ++x) {
                for (int y = 0; y < n; ++y) {
                    if (faceStatus[x][y][z][5] == 0) continue;

                    mask[x][y] = faceStatus[x][y][z][5];
                }
            }

            std::vector<StartEndPair> sePairs = GetGreedyFaces(mask, n);

            for (auto& pair : sePairs) {
                glm::ivec3 globalStart = glm::ivec3{ pair.start.x, pair.start.y, z } + (position * n);
                glm::ivec3 globalEnd = glm::ivec3{ pair.end.x, pair.end.y, z } + (position * n);

                glm::ivec2 texRepeatCount = pair.end - pair.start;
                texRepeatCount += 1;

                std::vector<Vertex> verts{
                    Vertex{ glm::vec3{ globalStart.x - 0.5f, globalStart.y - 0.5f, (float)z + (glm::vec3{ position } * (float)n).z - 0.5f }, glm::vec3{ 0.0f,  0.0f,  -1.0f }, glm::vec2{ 0.0f, 0.0f                         }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalEnd.x   + 0.5f, globalStart.y - 0.5f, (float)z + (glm::vec3{ position } * (float)n).z - 0.5f }, glm::vec3{ 0.0f,  0.0f,  -1.0f }, glm::vec2{ texRepeatCount.x, 0.0f             }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalEnd.x   + 0.5f, globalEnd.y   + 0.5f, (float)z + (glm::vec3{ position } * (float)n).z - 0.5f }, glm::vec3{ 0.0f,  0.0f,  -1.0f }, glm::vec2{ texRepeatCount.x, texRepeatCount.y }, (float)pair.blockIndex },
                    Vertex{ glm::vec3{ globalStart.x - 0.5f, globalEnd.y   + 0.5f, (float)z + (glm::vec3{ position } * (float)n).z - 0.5f }, glm::vec3{ 0.0f,  0.0f,  -1.0f }, glm::vec2{ 0.0f,             texRepeatCount.y }, (float)pair.blockIndex }
                };

                geo.vertices.insert(geo.vertices.end(), verts.begin(), verts.end());
            }
        }

        // Indices
        for (int i = 0; i < geo.vertices.size(); i += 4) {
            auto indices = faceIndices;

            for (auto& ind : indices) {
                ind += i;
            }

            geo.indices.insert(geo.indices.end(), indices.begin(), indices.end());
        }

        // TODO dont delete, will need this later for faster generation
        /*
		for (int x = 0; x < n; ++x) {
			for (int y = 0; y < n; ++y) {
				for (int z = 0; z < n; ++z) {
					if (grid[x][y][z] == 0) continue;

                    glm::mat4 translation{ 1.0f };
                    translation = glm::translate(translation, glm::vec3{
                        (float)(x + (position.x * n)) + 0.5f,
                        (float)(y + (position.y * n)) + 0.5f,
                        (float)(z + (position.z * n)) + 0.5f
                    });

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

                    std::vector<Vertex> vertices{ };

                    size_t faceCount{ 0 };

                    if (px == 0) {
                        vertices.insert(vertices.end(), pxVerts.begin(), pxVerts.end());
                        faceCount += 1;
                    }

                    if (py == 0) {
                        vertices.insert(vertices.end(), pyVerts.begin(), pyVerts.end());
                        faceCount += 1;
                    }

                    if (pz == 0) {
                        vertices.insert(vertices.end(), pzVerts.begin(), pzVerts.end());
                        faceCount += 1;
                    }

                    if (nx == 0) {
                        vertices.insert(vertices.end(), nxVerts.begin(), nxVerts.end());
                        faceCount += 1;
                    }

                    if (ny == 0) {
                        vertices.insert(vertices.end(), nyVerts.begin(), nyVerts.end());
                        faceCount += 1;
                    }

                    if (nz == 0) {
                        vertices.insert(vertices.end(), nzVerts.begin(), nzVerts.end());
                        faceCount += 1;
                    }

                    for (auto& v : vertices) {
                        glm::vec4 v4{ v.position.x, v.position.y, v.position.z, 1.0f };

                        v4 = translation * v4;

                        v.position = glm::vec3{ v4.x, v4.y, v4.z };

                        v.blockIndex = (float)(grid[x][y][z] - 1);
                    }

                    geo.vertices.insert(geo.vertices.end(), vertices.begin(), vertices.end());

                    std::vector<Index> indices{ };

                    for (size_t i = 0; i < faceCount; ++i) {
                        std::vector<Index> ind = faceIndices;

                        for (auto& in : ind) {
                            in += (Index)(4 * i);
                        }

                        indices.insert(indices.end(), ind.begin(), ind.end());
                    }

                    for (auto& i : indices) {
                        i += highestIndex;
                    }

                    highestIndex += (int)(faceCount * 4);

                    geo.indices.insert(geo.indices.end(), indices.begin(), indices.end());
				}
			}
		}
        */
        return geo;
	}

    std::vector<float> VerticesAsFloatVector(const std::vector<Vertex>& vertices) {
        std::vector<float> newVertices{ };

        newVertices.resize(vertices.size() * sizeof(Vertex));

        std::memcpy(newVertices.data(), vertices.data(), vertices.size() * sizeof(Vertex));

        return newVertices;
    }
}