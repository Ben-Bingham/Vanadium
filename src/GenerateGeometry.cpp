#include "GenerateGeometry.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Vanadium {
    std::vector<Vertex> cubeVertices {
        Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 0.0f, 0.0f } },
        Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 1.0f, 0.0f } },
        Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 1.0f, 1.0f } },
        Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  0.0f, -1.0f }, glm::vec2{ 0.0f, 1.0f } },

        Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 0.0f, 0.0f } },
        Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 1.0f, 0.0f } },
        Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 1.0f, 1.0f } },
        Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  0.0f,  1.0f }, glm::vec2{ 0.0f, 1.0f } },

        Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f }, glm::vec3{-1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f, 0.0f } },
        Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{-1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f, 1.0f } },
        Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{-1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f, 1.0f } },
        Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{-1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f, 0.0f } },

        Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f, 0.0f } },
        Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 1.0f, 1.0f } },
        Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f, 1.0f } },
        Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f }, glm::vec3{ 1.0f,  0.0f,  0.0f }, glm::vec2{ 0.0f, 0.0f } },

        Vertex{ glm::vec3{ -0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 0.0f, 1.0f } },
        Vertex{ glm::vec3{  0.5f, -0.5f, -0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 1.0f, 1.0f } },
        Vertex{ glm::vec3{  0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 1.0f, 0.0f } },
        Vertex{ glm::vec3{ -0.5f, -0.5f,  0.5f }, glm::vec3{ 0.0f, -1.0f,  0.0f }, glm::vec2{ 0.0f, 0.0f } },

        Vertex{ glm::vec3{ -0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 0.0f, 1.0f } },
        Vertex{ glm::vec3{  0.5f,  0.5f, -0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 1.0f, 1.0f } },
        Vertex{ glm::vec3{  0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 1.0f, 0.0f } },
        Vertex{ glm::vec3{ -0.5f,  0.5f,  0.5f }, glm::vec3{ 0.0f,  1.0f,  0.0f }, glm::vec2{ 0.0f, 0.0f } }
    };

    std::vector<Index> cubeIndices{
        2,  1,  0,
        0,  3,  2,

        4,  5,  6,
        6,  7,  4,

        8,  9, 10,
        10, 11, 8,

        14, 13, 12,
        12, 15, 14,

        16, 17, 18,
        18, 19, 16,

        22, 21, 20,
        20, 23, 22,
    };

	Geometry GenerateGeometry(const Grid& grid, int n, int atlasWidth, int atlasHeight) {
        Geometry geo{ };

        int highestIndex = 0;

		for (int x = 0; x < n; ++x) {
			for (int y = 0; y < n; ++y) {
				for (int z = 0; z < n; ++z) {
					if (grid[x][y][z] == 0) continue;

                    glm::mat4 translation{ 1.0f };
                    translation = glm::translate(translation, glm::vec3{ (float)x, (float)y, (float)z });

                    std::vector<Vertex> vertices = cubeVertices;
                    for (auto& v : vertices) {
                        glm::vec4 v4{ v.position.x, v.position.y, v.position.z, 1.0f };

                        v4 = translation * v4;

                        v.position = glm::vec3{ v4.x, v4.y, v4.z };
                    }

                    for (auto& v : vertices) {
                        glm::vec2 uv = v.uv;
                        BlockIndex id = grid[x][y][z];
                        int i = (int)id - 1;

                        int x = i % 2;
                        int y = (int)std::floor((float)i / 2.0f);

                        uv *= 0.5;

                        uv.x += 0.5 * x;
                        uv.y += 0.5 * y;

                        v.uv = uv;
                    }

                    geo.vertices.insert(geo.vertices.end(), vertices.begin(), vertices.end());

                    std::vector<Index> indices = cubeIndices;

                    for (auto& i : indices) {
                        i += highestIndex;
                    }

                    highestIndex += 24;

                    geo.indices.insert(geo.indices.end(), indices.begin(), indices.end());
				}
			}
		}

        return geo;
	}

    std::vector<float> VerticesAsFloatVector(const std::vector<Vertex>& vertices) {
        std::vector<float> newVertices{ };

        for (size_t i = 0; i < vertices.size(); ++i) {
            const Vertex& vert = vertices[i];
            newVertices.push_back(vert.position.x);
            newVertices.push_back(vert.position.y);
            newVertices.push_back(vert.position.z);

            newVertices.push_back(vert.normal.x);
            newVertices.push_back(vert.normal.y);
            newVertices.push_back(vert.normal.z);

            newVertices.push_back(vert.uv.x);
            newVertices.push_back(vert.uv.y);
        }

        return newVertices;
    }
}