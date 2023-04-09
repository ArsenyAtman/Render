#pragma once

#include <vulkan/vulkan.h>

#include "PhysicalDeviceManager.h"
#include "SwapChainManager.h"

class SwapChainManager;
class VertexBuffer;

class CommandManager
{

public:

	CommandManager(VkDevice logicalDevice, const QueueFamilyIndices& queueFamilyIndices);
	virtual ~CommandManager();

	void recordCommandBuffer(uint32_t imageIndex, SwapChainManager* swapChainManager, VkPipeline graphicsPipeline, VertexBuffer* vertexBuffer);

	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;

private:

	VkDevice logicalDevice;

	void createCommandPool(const QueueFamilyIndices& queueFamilyIndices);
	void createCommandBuffer();
};

