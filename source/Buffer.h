#pragma once

#include "RenderModule.h"

#include <vulkan/vulkan.h>

class Buffer : public RenderModule
{
public:

	Buffer(Render* render, Device* device, const ApplicationSettings* settings);
	virtual ~Buffer();

	virtual void bindBuffer(VkCommandBuffer commandBuffer) const { return; }

	uint32_t getBufferSize() const { return bufferSize; }

protected:

	void constructBuffer(const void* data, uint32_t elementSize, uint32_t countOfElements, VkBufferUsageFlagBits usageFlags);

	VkBuffer getBuffer() const { return buffer; }

private:

	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
	uint32_t bufferSize;
};

