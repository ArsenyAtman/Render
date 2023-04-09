#pragma once

#include <vulkan/vulkan.h>

class SyncsManager
{

public:

	SyncsManager(VkDevice logicalDevice);
	virtual ~SyncsManager();

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

private:

	VkDevice logicalDevice;
};

