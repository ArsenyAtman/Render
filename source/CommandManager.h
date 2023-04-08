#pragma once

#define GLFW_INCLUDE_VULKAN // Include Vulkan inside GLFW
#include <GLFW/glfw3.h>

#include "PhysicalDeviceManager.h"
#include "SwapChainManager.h"

class SwapChainManager;

class CommandManager
{

public:

	CommandManager(VkDevice logicalDevice, const QueueFamilyIndices& queueFamilyIndices);
	virtual ~CommandManager();

	void recordCommandBuffer(uint32_t imageIndex, SwapChainManager* swapChainManager, VkPipeline graphicsPipeline);

	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;

private:

	VkDevice logicalDevice;

	void createCommandPool(const QueueFamilyIndices& queueFamilyIndices);
	void createCommandBuffer();
};

