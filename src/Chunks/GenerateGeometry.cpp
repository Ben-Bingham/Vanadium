#include "GenerateGeometry.h"

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

        int highestIndex = 0;

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

        return geo;
	}

    std::vector<float> VerticesAsFloatVector(const std::vector<Vertex>& vertices) {
        std::vector<float> newVertices{ };

        newVertices.resize(vertices.size() * sizeof(Vertex));

        std::memcpy(newVertices.data(), vertices.data(), vertices.size() * sizeof(Vertex));

        return newVertices;
    }
}