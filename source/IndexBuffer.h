#pragma once

#include <vulkan/vulkan.h>

class Mesh;

class IndexBuffer
{
public:

	IndexBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, const Mesh* mesh);
	virtual ~IndexBuffer();

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkDevice logicalDevice;

	uint32_t indexBufferSize;
};

