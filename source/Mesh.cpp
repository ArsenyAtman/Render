#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices)
{
	this->vertices = vertices;
	
	for (size_t i = 0; i < vertices.size(); ++i)
	{
		indices.push_back(i);
	}
}