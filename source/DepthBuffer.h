#pragma once

#include <vulkan/vulkan.h>

class DepthBuffer
{

public:

	DepthBuffer(VkDevice logicalDevice, VkPhysicalDevice physicalDevice, VkQueue graphicsQueue, VkCommandPool commandPool, VkExtent2D swapChainExtent);
	virtual ~DepthBuffer();

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

private:

	VkDevice logicalDevice;
};

