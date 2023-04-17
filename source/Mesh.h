#pragma once

#include <vector>

#include "Vertex.h"

class Mesh
{
public:

	Mesh(std::vector<Vertex>& vertices);

	const std::vector<Vertex>& getVertices() const { return vertices; }
	const std::vector<uint32_t>& getIndices() const { return indices; }

private:

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

