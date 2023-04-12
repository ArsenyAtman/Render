#pragma once

#include <vector>
#include <cstdint>

#include "Vertex.h"

class ModelLoader
{
	
public:

	ModelLoader();

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

