#pragma once

#include "RenderModule.h"

#include <vulkan/vulkan.h>

class MeshBuffer : public RenderModule
{
public:

	MeshBuffer(Render* render, Device* device, const ApplicationSettings* settings);
	virtual ~MeshBuffer();

	virtual void bindBuffer(VkCommandBuffer commandBuffer) { return; }

	uint32_t getBufferSize() const { return bufferSize; }

protected:

	void constructBuffer(const void* data, uint32_t elementSize, uint32_t countOfElements, VkBufferUsageFlagBits usageFlags);

	VkBuffer getBuffer() const { return buffer; }

private:

	VkBuffer buffer;
	VkDeviceMemory bufferMemory;
	uint32_t bufferSize;
};

