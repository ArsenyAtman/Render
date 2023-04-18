#pragma once

#include <vector>

#include <vulkan/vulkan.h>

class Mesh;

class VertexBuffer
{

public:

	VertexBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, const Mesh* mesh);
	virtual ~VertexBuffer();

	VkDevice logicalDevice;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	uint32_t vertexBufferSize;
};

