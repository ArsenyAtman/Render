#include "CommandManager.h"

#include <stdexcept>
#include <array>

#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DescriptorsManager.h"

#include "Device.h"
#include "Settings.h"

CommandManager::CommandManager(VkDevice logicalDevice, const QueueFamilyIndices& queueFamilyIndices, const ApplicationSettings* settings)
{
	this->logicalDevice = logicalDevice;

	createCommandPool(queueFamilyIndices);
	createCommandBuffers(settings);
}

CommandManager::~CommandManager()
{
	vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
}

void CommandManager::createCommandPool(const QueueFamilyIndices& queueFamilyIndices)
{
	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	VkResult result = vkCreateCommandPool(logicalDevice, &poolInfo, nullptr, &commandPool);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create command pool!");
	}
}

void CommandManager::createCommandBuffers(const ApplicationSettings* settings)
{
	commandBuffers.resize(settings->maxFramesInFlight);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = settings->maxFramesInFlight;

	VkResult result = vkAllocateCommandBuffers(logicalDevice, &allocInfo, commandBuffers.data());
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate command buffers!");
	}
}

void CommandManager::recordCommandBuffer(uint32_t currentFrame, uint32_t imageIndex, SwapChainManager* swapChainManager, GraphicsPipeline* graphicsPipeline, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, DescriptorsManager* descriptorsManager)
{
	vkResetCommandBuffer(commandBuffers[currentFrame], 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VkResult result = vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChainManager->renderPass;
	renderPassInfo.framebuffer = swapChainManager->swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChainManager->swapChainExtent;

	std::array<VkClearValue, 2> clearValues{};
	clearValues[0].color = { {0.0f, 0.0f, 0.0f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };
	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	renderPassInfo.pClearValues = clearValues.data();

	vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainManager->swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainManager->swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainManager->swapChainExtent;
	vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);

	VkBuffer vertexBuffers[] = { vertexBuffer->vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffers[currentFrame], indexBuffer->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, 1, &descriptorsManager->descriptorSets[0], 0, nullptr);

	vkCmdDrawIndexed(commandBuffers[currentFrame], indexBuffer->indexBufferSize, 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffers[currentFrame]);

	VkResult result2 = vkEndCommandBuffer(commandBuffers[currentFrame]);
	if (result2 != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}