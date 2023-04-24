#include "Mesh.h"

#include <unordered_map>
#include <cstdint>

Mesh::Mesh(std::vector<Vertex>& meshVertices)
{
	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	
	for (const Vertex& vertex : meshVertices)
	{
		if (uniqueVertices.count(vertex) == 0)
		{
			uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
			vertices.push_back(vertex);
		}

		indices.push_back(uniqueVertices[vertex]);
	}
}
