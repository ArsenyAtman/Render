#include "CommandManager.h"

#include <stdexcept>

#include "GraphicsPipeline.h"
#include "VertexBuffer.h"

CommandManager::CommandManager(VkDevice logicalDevice, const QueueFamilyIndices& queueFamilyIndices)
{
	this->logicalDevice = logicalDevice;

	createCommandPool(queueFamilyIndices);
	createCommandBuffer();
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

void CommandManager::createCommandBuffer()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	VkResult result = vkAllocateCommandBuffers(logicalDevice, &allocInfo, &commandBuffer);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to allocate command buffers!");
	}
}

void CommandManager::recordCommandBuffer(uint32_t imageIndex, SwapChainManager* swapChainManager, GraphicsPipeline* graphicsPipeline, VertexBuffer* vertexBuffer)
{
	vkResetCommandBuffer(commandBuffer, 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
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

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapChainManager->swapChainExtent.width);
	viewport.height = static_cast<float>(swapChainManager->swapChainExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainManager->swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	VkBuffer vertexBuffers[] = { vertexBuffer->vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->pipelineLayout, 0, 1, &graphicsPipeline->descriptorSets[0], 0, nullptr);

	vkCmdDraw(commandBuffer, vertexBuffer->vertexBufferSize, 1, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	VkResult result2 = vkEndCommandBuffer(commandBuffer);
	if (result2 != VK_SUCCESS)
	{
		throw std::runtime_error("failed to record command buffer!");
	}
}