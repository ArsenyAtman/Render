#pragma once

#include "RenderModule.h"
#include "CommandBufferBindable.h"

#include <vulkan/vulkan.h>

class Buffer : public RenderModule, public CommandBufferBindable
{
public:

	Buffer(Render* render, Device* device, const ApplicationSettings* settings);
	virtual ~Buffer();

	uint32_t getBufferSize() const { return bufferSize; }

protected:

	void constructBuffer(const void* data, int elementSize, size_t countOfElements, VkBufferUsageFlagBits usageFlags);

	VkBuffer getBuffer() const { return buffer; }

private:

	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
	uint32_t bufferSize;
};

