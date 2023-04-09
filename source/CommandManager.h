#pragma once

#include <vulkan/vulkan.h>

#include "PhysicalDeviceManager.h"
#include "SwapChainManager.h"

class SwapChainManager;
class GraphicsPipeline;
class VertexBuffer;
class UniformBuffer;

class CommandManager
{

public:

	CommandManager(VkDevice logicalDevice, const QueueFamilyIndices& queueFamilyIndices);
	virtual ~CommandManager();

	void recordCommandBuffer(uint32_t currentFrame, SwapChainManager* swapChainManager, GraphicsPipeline* graphicsPipeline, VertexBuffer* vertexBuffer, UniformBuffer* uniformBuffer);

	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;

private:

	VkDevice logicalDevice;

	void createCommandPool(const QueueFamilyIndices& queueFamilyIndices);
	void createCommandBuffer();
};

