#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace Vanadium {
	using Index = unsigned int;

	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;

		float blockIndex;
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
}