#pragma once

#include "RenderModule.h"

#include <vector>

#include <vulkan/vulkan.h>

class Model;
class Buffer;

class CommandBuffer : public RenderModule
{
public:

	CommandBuffer(Render* render, Device* device, const ApplicationSettings* settings, const Model* model);
	virtual ~CommandBuffer();

	void recordCommandBuffer(uint32_t imageIndex);

	const VkCommandBuffer& getCommandBufferForCurrentFrame() const;

	const std::vector<Buffer*>& getBuffers() const { return buffers; }
	Buffer* getIndexBuffer() const { return indexBuffer; }

private:

	void createCommandPool();
	void createCommandBuffers();

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	std::vector<Buffer*> buffers;
	Buffer* indexBuffer = nullptr;
};
