#pragma once

#include "RenderModule.h"

#include <vector>

#include <vulkan/vulkan.h>

#include "SwapChainManager.h"

class Device;
class SwapChainManager;
class GraphicsPipeline;
class VertexBuffer;
class IndexBuffer;
class DescriptorsManager;

struct ApplicationSettings;

class CommandManager : public RenderModule
{

public:

	CommandManager(Render* render, Device* device, const ApplicationSettings* settings);
	virtual ~CommandManager();

	void recordCommandBuffer(uint32_t imageIndex);

	const VkCommandBuffer& getCommandBufferForCurrentFrame() const;

private:

	void createCommandPool();
	void createCommandBuffers();

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;
};

