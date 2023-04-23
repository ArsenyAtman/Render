#include "CommandManager.h"

#include <stdexcept>
#include <array>

#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DescriptorsManager.h"

#include "Render.h"
#include "Device.h"
#include "Settings.h"

CommandManager::CommandManager(Render* render, Device* device, const ApplicationSettings* settings) : RenderModule(render, device, settings)
{
	createCommandPool();
	createCommandBuffers();
}

CommandManager::~CommandManager()
{
	vkDestroyCommandPool(getDevice()->getLogicalDevice(), commandPool, nullptr);
}

void CommandManager::createCommandPool()
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = getDevice()->getQueueIndices().graphicsFamily.value();

	VkResult result = vkCreateCommandPool(getDevice()->getLogicalDevice(), &poolInfo, nullptr, &commandPool);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create command pool!");
	}
}

void CommandManager::createCommandBuffers()
{
	commandBuffers.resize(getSettings()->maxFramesInFlight);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = getSettings()->maxFramesInFlight;

	VkResult result = vkAllocateCommandBuffers(getDevice()->getLogicalDevice(), &allocInfo, commandBuffers.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate command buffers!");
	}
}

void CommandManager::recordCommandBuffer(uint32_t imageIndex)
{
	vkResetCommandBuffer(getCommandBufferForCurrentFrame(), 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VkResult commandBufferBeginResult = vkBeginCommandBuffer(getCommandBufferForCurrentFrame(), &beginInfo);
	if (commandBufferBeginResult != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to start recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = getRender()->getSwapChain()->renderPass;
	renderPassInfo.framebuffer = getRender()->getSwapChain()->swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = getRender()->getSwapChain()->swapChainExtent;

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(getCommandBufferForCurrentFrame(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	getRender()->getGraphicsPipeline()->bindToCommandBuffer(getCommandBufferForCurrentFrame());

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(getRender()->getSwapChain()->swapChainExtent.width);
	viewport.height = static_cast<float>(getRender()->getSwapChain()->swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(getCommandBufferForCurrentFrame(), 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = getRender()->getSwapChain()->swapChainExtent;
	vkCmdSetScissor(getCommandBufferForCurrentFrame(), 0, 1, &scissor);

	for (const Buffer* buffer : getRender()->getBuffers())
	{
		buffer->bindToCommandBuffer(getCommandBufferForCurrentFrame());
	}

	vkCmdDrawIndexed(getCommandBufferForCurrentFrame(), getRender()->getIndexBuffer()->getBufferSize(), 1, 0, 0, 0);

	vkCmdEndRenderPass(getCommandBufferForCurrentFrame());

	VkResult commandBufferEndResult = vkEndCommandBuffer(getCommandBufferForCurrentFrame());
	if (commandBufferEndResult != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to end recording command buffer!");
	}
}

const VkCommandBuffer& CommandManager::getCommandBufferForCurrentFrame() const
{
	return commandBuffers[getRender()->getCurrentFrame()];
}