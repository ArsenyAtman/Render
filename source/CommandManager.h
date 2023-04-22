#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "SwapChainManager.h"

class SwapChainManager;
class GraphicsPipeline;
class VertexBuffer;
class IndexBuffer;
class DescriptorsManager;

struct ApplicationSettings;

class CommandManager
{

public:

	CommandManager(VkDevice logicalDevice, const QueueFamilyIndices& queueFamilyIndices, const ApplicationSettings* settings);
	virtual ~CommandManager();

	void recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex, SwapChainManager* swapChainManager, GraphicsPipeline* graphicsPipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, DescriptorsManager* descriptorsManager);

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

private:

	VkDevice logicalDevice;

	void createCommandPool(const QueueFamilyIndices& queueFamilyIndices);
	void createCommandBuffers(const ApplicationSettings* settings);
};

