#pragma once

#include <vector>

#include <vulkan/vulkan.h>

struct ApplicationSettings;

class SyncsManager
{

public:

	SyncsManager(VkDevice logicalDevice, const ApplicationSettings* settings);
	virtual ~SyncsManager();

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;

private:

	VkDevice logicalDevice;
};

