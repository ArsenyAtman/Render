#pragma once

#include <vulkan/vulkan.h>

class CommandBufferBindable
{
public:

	virtual void bindToCommandBuffer(VkCommandBuffer commandBuffer) const = 0;
};

