#pragma once

#include "RenderModule.h"

#include <vulkan/vulkan.h>

class DepthBuffer : public RenderModule
{
public:

	DepthBuffer(Render* render, Device* device, const ApplicationSettings* settings, VkExtent2D swapChainExtent);
	virtual ~DepthBuffer();

	const VkImageView& getImageView() { return depthImageView; }

private:

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;
};
