#pragma once

#include <glm/glm.hpp>

#include "GridGeneration.h"

namespace Vanadium {
	using Index = unsigned int;

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	struct Geometry {
		// p -> positive
		// n -> negative

		// x -> x face
		// y -> y face
		// z -> z face

		// V -> vertices
		// I -> indices

		//std::vector<Vertex> pxV;
		//std::vector<Index> pxI;

		//std::vector<Vertex> pyV;
		//std::vector<Index> pyI;

		//std::vector<Vertex> pzV;
		//std::vector<Index> pzI;

		//std::vector<Vertex> nxV;
		//std::vector<Index> nxI;

		//std::vector<Vertex> nyV;
		//std::vector<Index> nyI;

		//std::vector<Vertex> nzV;
		//std::vector<Index> nzI;

		std::vector<Vertex> vertices;
		std::vector<Index> indices;
	};

	Geometry GenerateGeometry(const Grid& grid, int n);

	std::vector<float> VerticesAsFloatVector(const std::vector<Vertex>& vertices);
}