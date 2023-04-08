#pragma once

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

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

