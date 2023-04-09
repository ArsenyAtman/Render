#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "Vertex.h"

class VertexBuffer
{

public:

	VertexBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, const std::vector<Vertex>& vertices);
	virtual ~VertexBuffer();

	VkDevice logicalDevice;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	uint32_t vertexBufferSize;
};

