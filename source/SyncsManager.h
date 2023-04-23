#pragma once

#include "RenderModule.h"

#include <vector>

#include <vulkan/vulkan.h>

class SyncsManager : public RenderModule
{

public:

	SyncsManager(Render* render, Device* device, const ApplicationSettings* settings);
	virtual ~SyncsManager();

	VkSemaphore getImageAvailableSemaphoreForCurrentFrame() const;
	VkSemaphore getRenderFinishedSemaphoreForCurrentFrame() const;
	VkFence getInFlightFenceForCurrentFrame() const;

private:

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
};

